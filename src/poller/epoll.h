#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <fcntl.h>
#include <unistd.h>
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
        _epoll_fd = epoll_create(size); // error!!!
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

        ev.data.fd = hd->get_fd();
        ev.events = EPOLLIN | EPOLLET;
        epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, hd->get_fd(), &ev); //error!!!

        return E_OK;
    }

    virtual int poll(const int max_event_size = 100, const int timeout = -1)
    {
        struct epoll_event *events = new struct epoll_event[max_event_size];

        int fd_size = 0, i = 0;
        while(true)
        {
            fd_size = epoll_wait(_epoll_fd, events, max_event_size, timeout);

            for(i = 0; i < fd_size; ++i)
            {
                if(events[i].data.fd==listenfd)
                {
                    //循环accept
                    while((connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen)) > 0)
                    {
                        setnonblocking(connfd); //把客户端的socket设置为非阻塞方式

                        char *str = inet_ntoa(clientaddr.sin_addr);
                        std::cout<<"connect from "<<str<<std::endl;

                        ev.data.fd=connfd; //设置用于读操作的文件描述符
                        ev.events=EPOLLIN | EPOLLET; //设置用于注测的读操作事件
                        epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev); //注册ev事件
                    }
                }
                else if(events[i].events&EPOLLIN)     /**读事件**/
                {
                    fprintf(stderr, "EPOLLIN: %d, fd: %d\n", maxi++, events[i].data.fd);
                    if ( (sockfd = events[i].data.fd) < 0)
                    {
                        continue;
                    }
                    memset(line, 0, MAXLINE);
                    n = 0;
                    int nread = 0;
                    while((nread= read(sockfd, line + n, MAXLINE)) > 0)
                    {
                        n += nread;
                    }//读到EAGAIN，说明读完了

                    if(nread == -1 && errno != EAGAIN)
                    {
                        epoll_ctl_err_show();
                        std::cout<<"readline error"<<std::endl;
                        close(sockfd); //关闭一个文件描述符，那么它会从epoll集合中自动删除
                        //描述符关闭后，后面的邋邋邋邋EPOLLOUT设置了，但不起作用了
                        events[i].data.fd = -1;
                    }

                    //这里要加上判断，nread为0时，说明客户端已经关闭
                    //此时，需要关闭描述符，否则在/proc/id/fd下能看到描述符会一直存在
                    char *str = inet_ntoa(clientaddr.sin_addr);
                    if(nread == 0)
                    {
                        close(sockfd);
                        std::cout << "close from " << str << std::endl;
                        continue;
                    }

                    std::cout << "read from " << str << ": " << line << std::endl;

                    ev.data.fd=sockfd; //设置用于写操作的文件描述符
                    ev.events=EPOLLOUT | EPOLLET; //设置用于注测的写操作事件
                    epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev); //修改sockfd上要处理的事件为EPOLLOUT
                }
                else if(events[i].events & EPOLLOUT)    /**写事件**/
                {
                    sockfd = events[i].data.fd;
                    //write(sockfd, line, n); orig

                    int iRet = socket_send(sockfd, line, strlen(line) + 1);
                    if(iRet == -1 || iRet != strlen(line) + 1)
                    {
                        perror("write error!");
                    }/*zxd*/

                    ev.data.fd=sockfd; //设置用于读操作的文件描述符
                    ev.events=EPOLLIN | EPOLLET; //设置用于注册的读操作事件
                    epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev); //修改sockfd上要处理的事件为EPOLIN
                }
            }
        }
    }
};

#endif
