#ifndef _EC_SOCKET_H_
#define _EC_SOCKET_H_

namespace easycxx
{

typedef (*accept_callback)(std::string remote_host, int remote_port);
//typedef (*recv_callback)(void *buffer, int buffer_length);


class ESocket
{
public:
	ESocket(){}
	virtual ~ESocket(){}
};

}

#endif

