#ifndef _EC_SOCKET_H_
#define _EC_SOCKET_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "esocket_errors.h"
#include "byte_buffer.h"

namespace easycxx
{

typedef int (*accept_callback)(const int &client_socket, const std::string &remote_host, const int &remote_port);
//typedef (*recv_callback)(void *buffer, int buffer_length);


class ESocket
{
public:
	ESocket(){}
	virtual ~ESocket(){}

	virtual int connect(std::string remote_host, int remote_port) = 0;
	//virtual int be_server(std::string remote_host, int remote_port) = 0;
	virtual int be_server() = 0;
	//virtual int accept(accept_callback fcallback) = 0;
	virtual int accept() = 0;
	virtual int send(EByteBuffer &byte_buffer) = 0;
	//virtual int recv(EByteBuffer &byte_buffer) = 0;
	virtual int recv(int conn, EByteBuffer &byte_buffer) = 0;
	//virtual int recv_asyn(EByteBuffer &byte_buffer);
	//virtual int close() = 0; // close self
	virtual int close(const int conn) = 0;

	virtual const int get_last_error() = 0;
};

}

#endif

