#ifndef _EC_HANDLER_H_
#define _EC_HANDLER_H_

#include "arch.h"

using namespace std;

namespace ec
{

class Handle
{
public:
	Handle():{}
	virtual ~Handle(){}
	virtual int get_handle() = 0;
	virtual int create() = 0;
};

//TODO: class FileHandle;
//TODO: class TimeHandle;
class SocketHandle
{
private:
	int _fd;

public:
	SocketHandle();
	virtual ~SocketHandle();

	virtual int get_handle();
	virtual int create();
};

class EventHandler
{
private:
	shared_ptr<Handle> _handle;
	int _status;

public:
	EventHandler();
	EventHandler(shared_ptr<Handle> &handle);
	virtual ~EventHandler();

	inline void set_handle(shared_ptr<Handle> &handle);
	inline int get_handle();
	virtual int handle_event();
};

}; // namespace ec

#endif

