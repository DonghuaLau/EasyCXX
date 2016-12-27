#ifndef _EC_TCP_SOCKET_H_
#define _EC_TCP_SOCKET_H_

#include "easycxx.h"

namespace easycxx
{

class ETCPSocket
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
	ETCPSocket();
	ETCPSocket(std::string host, int port);
	virtual ~ETCPSocket();

	void set_backlog(int backlog)
	{
		_backlog = backlog;
	}

	virtual int connect(std::string remote_host, int remote_port); // client
	virtual int be_server(std::string remote_host, int remote_port); // server
	virtual int accept(accept_callback fcallback);
	virtual int send(void *buffer, int &buffer_length);
	virtual int recv(void **buffer, int &buffer_length);
	virtual int recv_asyn(void **buffer, int &buffer_length);
};

}

#endif

