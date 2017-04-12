#ifndef _EC_TCP_SOCKET_H_
#define _EC_TCP_SOCKET_H_

#include "socket.h"

namespace easycxx
{

class TCPSocket : public Socket
{
private:
	int _socket;
	int _domain;
	int _type;
	int _protocol;
	int _backlog;

	std::string _host; // ip string
	int _port;

	std::string _remote_host;
	int _remote_port;

	int _last_error;

private:
	int init();

public:
	TCPSocket();
	TCPSocket(std::string host, int port);
	virtual ~TCPSocket();

	void set_backlog(int backlog)
	{
		_backlog = backlog;
	}

	virtual int connect(std::string remote_host, int remote_port); // client
	//virtual int be_server(std::string remote_host, int remote_port); // server
	virtual int be_server(); // server
	//virtual int accept(accept_callback fcallback);
	virtual int accept();
	virtual int send(EByteBuffer &byte_buffer);
	//virtual int recv(EByteBuffer &byte_buffer);
	virtual int recv(int conn, EByteBuffer &byte_buffer);
	//virtual int recv_asyn(EByteBuffer &byte_buffer);
	//virtual int close(); // close self
	virtual int close(const int conn); // close client

	const int get_last_error()
	{
		return _last_error;
	}
	
	bool get_remote_host_and_port(std::string &host, int &port)
	{	
		host = _remote_host;
		port = _remote_port;
	}
	
	int get_fd()
	{
		return _fd;
	}
};

}

#endif

