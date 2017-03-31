#ifndef _EC_SOCKET_H_
#define _EC_SOCKET_H_

#include "esocket.h"

namespace easycxx
{

class Connection
{
public:
	Connection()
	~Connection()

private:
	int _conn;
	Socket *_socket;
}

}

#endif
