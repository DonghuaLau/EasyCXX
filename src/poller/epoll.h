#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "poller.h"

class EPoll
{
private:
    FD _epoll_fd;
    map<FD, *Handler> _client_handlers;
    map<FD, *Handler> _listen_handlers;
public:
    EPoll(const int size = 256)
    {
        _epoll_fd = epoll_create(size); // TODO: error!!!
    }

    int add(Handler *hd, bool is_listen = false)
    {
        if(hd == NULL || hd->get_fd() <= 0)
        {
            return E_INVALID;
        }

        if(is_listen)
        {
            if(_client_handlers.find(hd->get_fd()) != _client_handlers.end())
            {
                return E_EXIST;
            }
            _client_handlers.insert(pair<FD, *Handler>(hd->get_fd(), hd));
        }
        else
        {
            if(_listen_handlers.find(hd->get_fd()) != _listen_handlers.end())
            {
                return E_EXIST;
            }
            _listen_handlers.insert(pair<FD, *Handler>(hd->get_fd(), hd));
        }

        struct epoll_event ev;
        ev.data.fd = hd->get_fd();
        ev.events = EPOLLIN | EPOLLET;
        epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, hd->get_fd(), &ev); // TODO: error!!!

        return E_OK;
    }

    virtual int poll(const int max_event_size = 100, const int timeout = -1)
    {
        struct epoll_event *events = new struct epoll_event[max_event_size];
        struct epoll_event ev;

        int fd_size = 0, i = 0;
        map<FD, *Handler>::iterator it;
        FD conn_fd = 0, sock_fd = 0;
        struct sockaddr_in client_addr;
        socklen_t sock_len;
        char buf[MAX_SOCK_BUF_SIZE];
        int total = 0, nread = 0, nwrite = 0;

        while(true)
        {
            fd_size = epoll_wait(_epoll_fd, events, max_event_size, timeout);

            for(i = 0; i < fd_size; ++i)
            {
                it = _listen_handlers.find(events[i].data.fd);
                if(it != _listen_handlers.end())
                {
                    if(it->second == NULL) continue;
                    FD & listenfd =it->second->get_fd();
                    while((conn_fd = accept(listenfd,(sockaddr *)&client_addr, &sock_len)) > 0) //accept loop
                    {
                        if(setnonblocking(conn_fd) != 0) continue;

                        char *str = inet_ntoa(client_addr.sin_addr);
                        std::cout << "connect from " << str << std::endl;

                        ev.data.fd = conn_fd;
                        ev.events = EPOLLIN | EPOLLET;
                        epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev); // TODO: error!!!

                        // TODO: add to map and create Handler
                    }
                }
                else if(events[i].events & EPOLLIN)     /* read event */
                {
                    if ((sock_fd = events[i].data.fd) < 0)
                    {
                        continue;
                    }

                    it = _client_handlers.find(sock_fd);
                    if(it == _client_handlers.end())
                    {
                        std::cout << "not found socket while read, and close it, fd: " << sock_fd << std::endl;
                        close(sock_fd);
                        events[i].data.fd = -1; // makes epoll auto delete this fd?
                        //epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, sock_fd, &events[i]); // TODO: delete from epoll?
                        continue;
                    }

                    //memset(buf, 0, MAX_SOCK_BUF_SIZE); // not recommended
                    buf[0] = 0, total = 0, nread = 0;

                    while((nread= read(sock_fd, buf + total, MAX_SOCK_BUF_SIZE)) > 0)
                    {
                        n += nread;
                    } //read untill EAGAIN error, means read completed.

                    if(nread == -1 && errno != EAGAIN)
                    {
                        std::cout << "read socket error: " << errno << ", fd: " << sock_fd << std::endl;
                        close(sock_fd);
                        events[i].data.fd = -1; // makes epoll auto delete this fd?
                        //epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, sock_fd, &events[i]); // TODO: delete from epoll?
                        continue;
                    }

                    if(nread == 0) // client is closed
                    {
                        close(sock_fd);
                        std::cout << "the client is colsed, we close either, addr: " << inet_ntoa(client_addr.sin_addr) << std::endl;
                        continue;
                    }

                    std::cout << "read from " << inet_ntoa(client_addr.sin_addr) << ", size: " << total << std::endl;

                    ev.data.fd = sock_fd;
                    ev.events = EPOLLOUT | EPOLLET;
                    epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, sock_fd, &ev); // TODO: error

                    it->second->input(buf, total); // TODO: coroutine
                }
                else if(events[i].events & EPOLLOUT)     /* write event */
                {
                    if ((sock_fd = events[i].data.fd) < 0)
                    {
                        continue;
                    }

                    it = _client_handlers.find(sock_fd);
                    if(it == _client_handlers.end())
                    {
                        std::cout << "not found socket while write, and close it, fd: " << sock_fd << std::endl;
                        close(sock_fd);
                        events[i].data.fd = -1; // makes epoll auto delete this fd?
                        //epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, sock_fd, &events[i]); // TODO: delete from epoll?
                        continue;
                    }

                    total = it->second->output(buf, MAX_SOCK_BUF_SIZE); // TODO: coroutine

                    nwrite = socket_send(sock_fd, buf, total + 1);
                    if(nwrite == -1 || nwrite != total + 1)
                    {
                        std::cout << "write socker error: " << errno << ", fd: " << sock_fd << std::endl;
                        // TODO: close socket?
                    }

                    ev.data.fd = sock_fd;
                    ev.events = EPOLLIN | EPOLLET;
                    epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, sock_fd, &ev);

                    // TODO: close socket if it's short connection
                }
            }
        }
    }

private:
    int setnonblocking(int sock)
    {
        int opts = fcntl(sock, F_GETFL);
        if(opts < 0)
        {
            cerr << "fcntl(sock, GETFL) error, errno: " << errno << endl;
            return -1;
        }
    
        opts = opts | O_NONBLOCK;
    
        if(fcntl(sock, F_SETFL, opts) < 0)
        {
            cerr << "fcntl(sock, F_SETFL, opts) error, errno: " << errno << endl;
            return -1;
        }
        return 0;
    }
    
    ssize_t socket_send(int sock_fd, const char *buffer, size_t buflen)
    {
        ssize_t n = 0;
        size_t total = buflen;
        const char *p = buffer;
    
        while(true)
        {
            n = send(sock_fd, p, total, 0);
            if(n < 0)
            {
                if(errno == EINTR) continue;
    
                if(errno == EAGAIN) // write cache queue if full
                {
                    usleep(1000); // TODO: seems not fine
                    continue;
                }
    
                return -1;
            }
    
            if((size_t)n == total) return buflen;
    
            total -= n;
            p += n;
        }
    
        return buflen - total;
    }
};

#endif
