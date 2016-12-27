#include "easycxx.h"

namespace easycxx
{

ETCPSocket::ETCPSocket():
{
	_host = "0.0.0.0";
	_port = 0;
	init();
}

ETCPSocket::ETCPSocket(std::string host, int port):
	_host(host), _port(port)
{
	init();
}

ETCPSocket::~ETCPSocket()
{
}

int ETCPSocket::init()
{
	_backlog = 10;

	_domain = AF_INET;
	_type = SOCK_STREAM;
	_protocol = 0;
	_socket = socket(_domain, _type, _protocol);
	if(_socket == -1)
	{
		_last_error = errno;
		return _last_error;
	}
	return 0;
}

int ETCPSocket::connect(std::string remote_host, int remote_port)
{
	_remote_host = remote_host;
	_remote_port = remote_port;

	struct sockaddr_in remote_addr;
	remote_addr.sin_family = AF_INET;
	remote_addr.sin_port = htons(_remote_port);
	if( inet_pton(AF_INET, _remote_host.c_str(), &remote_addr.sin_addr) <= 0 )
	{
		std::cerr << "inet_pton error, remote host: " << remote_host << ", remote port: " << remote_port << std::endl;
		_last_error = errno;
		return SOCK_ERR_INET_PTON;
	} 

	if( connect(_socket, (struct sockaddr *)&remote_addr, sizeof(remote_addr)) < 0 )
	{
		std::cerr << "socket connect failed, remote host: " << remote_host << ", remote port: " << remote_port << std::endl;
		_last_error = errno;
		return SOCK_ERR_CONNECT;
	} 

	return OK;
}

int ETCPSocket::be_server()
{
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_port);
	if( inet_pton(AF_INET, _host.c_str(), &server_addr.sin_addr) <= 0 )
	{
		std::cerr << "inet_pton error, host: " << _host << ", port: " << _port << std::endl;
		_last_error = errno;
		return SOCK_ERR_INET_PTON;
	} 

	if( bind(_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0 )
	{
		std::cerr << "socket bind error, host: " << _host << ", port: " << _port << std::endl;
		_last_error = errno;
		return _last_error;
	}

	if( listen(_socket, _backlog) != 0 )
	{
		std::cerr << "socket listen error, host: " << _host << ", port: " << _port << std::endl;
		_last_error = errno;
		return _last_error;
	}

	return OK;
}

int ETCPSocket::accept(accept_callback fcallback)
{
	struct sockaddr client_addr;
	int connfd = accept(listenfd, (struct sockaddr*)&client_addr, sizeof(client_addr)); 

	snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));

	return connfd;
}

int ETCPSocket::send(void *buffer, int &buffer_length)
{
	return OK;
}

int ETCPSocket::recv(void **buffer, int &buffer_length)
{
	return OK;
}

int ETCPSocket::recv_asyn(void **buffer, int &buffer_length)
{
}

}
