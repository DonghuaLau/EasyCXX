#include "handler.h"

SocketHandle::SocketHandle()
	:_fd(0)
{
}

SocketHandle::~SocketHandle()
{
	if(_fd > 0)
	{
		close(_fd);
		_fd = 0;
	}
}

int SocketHandle::get_handle()
{
	return _fd;
}

virtual int SocketHandle::create()
{
	//_fd = socket();
	return _fd;
}
