#ifndef _EC_SOCKET_H_
#define _EC_SOCKET_H_

#include "esocket.h"

namespace easycxx
{

class Connection
{
public:
	Connection();
	~Connection();

	int get_conn();

private:
	//int _fd; // equal to Socket::_socket
	Socket *_socket;
	Processor *_processor;

	bool _open;

	int _last_error;
};

}

#endif
