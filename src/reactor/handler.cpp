#include "handler.h"

EventHandler::EventHandler()
{
}

EventHandler::EventHandler(shared_ptr<Handle> &handle)
	:_handle(handle)
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::set_handle(shared_ptr<Handle> &handle)
{
	_handle = handle;
}

int EventHandler::get_handle()
{
	return _handle.get_handle();
}

int EventHandler::handle_event()
{
	// shared_ptr<BufferList> &i, shared_ptr<BufferList> &o
	o = i;
	// read and write
	return 0;
}
