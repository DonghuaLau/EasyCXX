#include "connection.h"

namespace easycxx
{


Connection::Connection()
{
}

Connection::~Connection()
{
}

int Connection::get_conn()
{
	if(!_open)
	{
		return SOCK_ERR_NOT_OPEN;
	}

	if(_socket == NULL)
	{
		return SOCK_ERR_NULL;
	}

	return _socket->get_fd();
}


}
