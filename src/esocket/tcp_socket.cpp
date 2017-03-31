#include "tcp_socket.h"

#include "errors.h"

namespace easycxx
{

TCPSocket::TCPSocket()
{
	_host = "0.0.0.0";
	_port = 80;
	init();
}

TCPSocket::TCPSocket(std::string host, int port):
	_host(host), _port(port)
{
	init();
}

TCPSocket::~TCPSocket()
{
	::close(_socket);
}

int TCPSocket::init()
{
	_backlog = 10;

	_domain = AF_INET;
	_type = SOCK_STREAM;
	_protocol = 0;
	_socket = ::socket(_domain, _type, _protocol);
	if(_socket == -1)
	{
		_last_error = errno;
		return _last_error;
	}
	return 0;
}

int TCPSocket::connect(std::string remote_host, int remote_port)
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

	if( ::connect(_socket, (struct sockaddr *)&remote_addr, sizeof(remote_addr)) < 0 )
	{
		std::cerr << "socket connect failed, remote host: " << remote_host << ", remote port: " << remote_port << std::endl;
		_last_error = errno;
		return SOCK_ERR_CONNECT;
	} 

	return SCOK_OK;
}

int TCPSocket::be_server()
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

	return SCOK_OK;
}

//int TCPSocket::accept(accept_callback fcallback)
int TCPSocket::accept()
{
	struct sockaddr client_addr;
	unsigned int client_addr_len = sizeof(client_addr);
	int connfd = ::accept(_socket, (struct sockaddr*)&client_addr, &client_addr_len ); 

	//snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));

	return connfd;
}

int TCPSocket::send(EByteBuffer &byte_buffer)
{
	ssize_t write_size = ::write(_socket, byte_buffer.get_buffer(), byte_buffer.size());
	std::cout << "[" << __func__ << "] send size: " << write_size << ", buffer size: " << byte_buffer.size() << std::endl;
	if(write_size == -1)
	{
		std::cerr << "socket send failed." << std::endl;
		_last_error = errno;
	}

	return write_size;
}

/*
 *	if BUFFER_NOT_ENOUGH occurs, call again, it's not an error.
 */
int TCPSocket::recv(int conn, EByteBuffer &byte_buffer)
{
	std::cout << "[" << __func__ << "] conn: " << conn << std::endl;
	byte pbuffer[4096];
	static size_t count = 4096;
	ssize_t read_size = 0;
	ssize_t total_size = 0;
	int push_size = 0;
	do
	{
		read_size = ::read(conn, pbuffer, count);
		if(read_size <= 0)
		{
			_last_error = errno;
			break;
		}

		// checking buffer's remain size first is better
		if(byte_buffer.get_remain_size() < read_size)
		{
			std::cout << "socket recv, buffer is not enough." << std::endl;
			_last_error = BUFFER_NOT_ENOUGH;
			break;
		}


		total_size += read_size;
		push_size = byte_buffer.push_back(pbuffer, read_size);
		std::cout << "[" << __func__ << "] push size: " << push_size << ", read size: " << read_size << std::endl;
		if(push_size < read_size)
		{
			// if this happens, it's a serious error. Data'll be lost. 
			std::cerr << "socket recv, buffer error." << std::endl;
			_last_error = BUFFER_NOT_ENOUGH;
			break;
		}
	}
	while(true);

	if(read_size < 0)
	{
		std::cerr << "socket recv failed, errno: "  << errno << std::endl;
		return ERROR;
	}

	return total_size;
}

//int TCPSocket::close()
//{
//	return ::close(_socket);
//}

int TCPSocket::close(const int conn)
{
	return ::close(conn);
}

//int TCPSocket::recv_asyn(EByteBuffer &byte_buffer)
//{
//	return SCOK_OK;
//}

}
