#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include "handle.h"

namespace ec
{

TCPHandle::TCPHandle()
	:_fd(0)
{
}

TCPHandle::~TCPHandle()
{
	if(_fd > 0)
	{
		::close(_fd);
		_fd = 0;
	}
}

inline int TCPHandle::get_handle()
{
	return _fd;
}

inline void TCPHandle::set_read_size(const int read_size)
{
	_read_size = read_size;
}

inline int TCPHandle::set_nonblock(int fd) 
{
	int flags = fcntl(fd, F_GETFL, 0);
	int ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	return ret;
}

int TCPHandle::listen(const string &ip, const int port)
{
	int ret = 0;

	_BLOCK_START_

    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
	BREAK_FAILED(_fd < 0, ret = EventError::E_SCOK_CREATE);
	set_nonblock(_fd);

    struct sockaddr_in serveraddr;
    bzero(&serveraddr, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
	if(!ip.empty())
	{
    	inet_aton(ip.c_str(), &(serveraddr.sin_addr));
	}
	else
	{
    	serveraddr.sin_addr.s_addr = INADDR_ANY;
	}

    ret = ::bind(_fd,(sockaddr *)&serveraddr, sizeof(serveraddr));
	BREAK_FAILED(ret < 0, ret = EventError::E_SCOK_BIND);

    //ret = listen(listenfd, LISTENQ); // LISTENQ ???
    ret = ::listen(_fd, DEFAULT_TCP_BACKLOG);
	BREAK_FAILED(ret < 0, ret = EventError::E_SCOK_LISTEN);

	_BLOCK_END_

	return ret;
}

int TCPHandle::accept(shared_ptr<Handle> &client_handle)
{
	//client_handle = make_shared<TCPHandle>();
	shared_ptr<TCPHandle> handle(new TCPHandle());
	//int client_fd = 0;
    struct sockaddr_storage sa;
    socklen_t salen = sizeof(sa);
    while(true) 
	{
        handle->_fd = ::accept(_fd, (struct sockaddr*)&sa, &salen);
        if (handle->_fd == -1) 
		{
            if (errno == EINTR)
			{
                continue;
			}
            else
			{
                elog << "accept error " << errno << ": " << strerror(errno) << ", fd: " << handle->_fd << endl;
                return EventError::E_SCOK_ACCEPT;
            }
        }
        break;
    }

	int port = 0;
	char ip[NET_IP_STR_LEN] = {0};
    if (sa.ss_family == AF_INET) 
	{
		handle->_ip_version = IPV4;
        struct sockaddr_in *s = (struct sockaddr_in *)&sa;
        inet_ntop(AF_INET, (void*)&(s->sin_addr), ip, sizeof(ip));
        port = ntohs(s->sin_port);
    } else {
		handle->_ip_version = IPV6;
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&sa;
        inet_ntop(AF_INET6, (void*)&(s->sin6_addr), ip, sizeof(ip));
        port = ntohs(s->sin6_port);
    }

	handle->_ip = ip;
	handle->_port = port;

	client_handle = handle;

	set_nonblock(handle->_fd);

	return 0;
}

int TCPHandle::read(shared_ptr<BufferList> &buf_list)
{
    int ret = 0;
    int n = 0, total = 0;
    while (true)
	{
		shared_ptr<Buffer> buf(new Buffer(_read_size));
    	n = ::read(_fd, buf->get_ptr(), _read_size);
		if(n < 0)
		{
    		if(errno == EAGAIN || errno == EWOULDBLOCK)
			{
				wlog << "read socket error[" << errno << "]: " << strerror(errno) << endl;
				//TODO
			}
			else
			{
				//TODO: EINTR
				elog << "read socket error[" << errno << "]: " << strerror(errno) << endl;
				ret = EventError::E_SOCK_READ;
			}
			break;
		}

		total += n;
		buf->set_length(n);
		buf_list->append(buf);
		dlog << "fd[" << _fd << "] read size: " << n << ", total read size: " << total << endl;

        //int r = ::write(fd, buf, n); //写出读取的数据
        //实际应用中，写出数据可能会返回EAGAIN，此时应当监听可写事件，当可写时再把数据写出
    }

    //exit_if(n<0, "read error"); //实际应用中，n<0应当检查各类错误，如EINTR

	return ret;
}

int TCPHandle::write(shared_ptr<BufferList> &buf_list)
{
	int ret = 0, n = 0;;
	list<shared_ptr<Buffer> >::iterator it = buf_list->get_list().begin();
	for(; it != buf_list->get_list().end(); it++)
	{
		shared_ptr<Buffer> &buf = *it;
		n = ::write(_fd, buf->get_ptr(), buf->get_length());
		if(n < 0 && errno != EAGAIN)
		{
			//TODO
			elog << "write socket error[" << errno << "]: " << strerror(errno) << endl;
			ret = EventError::E_SOCK_READ;
			break;
		}
	}
    return ret;
}

}; // namespace ec
