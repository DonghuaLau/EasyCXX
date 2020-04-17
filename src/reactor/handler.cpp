#include "handler.h"
#include "dispatcher.h"

namespace ec
{

EventHandler::EventHandler(bool is_svr_handle/* = false*/)
	:_is_svr_handle(is_svr_handle)
{
}

/*EventHandler::EventHandler(shared_ptr<Handle> &handle)
	:_handle(handle)
{
}*/

EventHandler::~EventHandler()
{
}

inline void EventHandler::set_handle(shared_ptr<Handle> &handle)
{
	_handle = handle;
}

inline int EventHandler::get_handle()
{
	return _handle->get_handle();
}

inline void EventHandler::set_link_type(const int link_type)
{
	_link_type = link_type;
}
inline int EventHandler::get_link_type()
{
	return _link_type;
}
inline void EventHandler::set_event_type(const int event_type)
{
	_event_type = event_type;
}
inline int EventHandler::get_event_type()
{
	return _event_type;
}

int EventHandler::server_init(const string &ip, const int port)
{
	int ret = 0;
	if(_event_type == EVENT_TCP)
	{
		shared_ptr<TCPHandle> handle(new TCPHandle());
		ret = handle->listen(ip, port);
		RETURN_FAILED(ret == 0, ret);
		//_event_dispatcher.register_server(*this); // call when defined EventDispatcher and EventHandler implementation
		//RETURN_FAILED(ret == 0, ret);
	}
	else
	{
		ret = EventError::E_EVENT_UNSPPORTED;
		elog << "event_type: " << _event_type << " not supported yet." << endl;
	}
	return ret;
}

int EventHandler::handle_accept(EventDispatcher &dispatcher, const int mask)
{
	int ret = 0;
	int client_fd;

	_BLOCK_START_

	shared_ptr<Handle> client_handle;
	ret = _handle->accept(client_handle);
	BREAK_FAILED(ret == 0, NULL);

	shared_ptr<EventHandler> handler = create_handler(client_handle);
	if(handler != nullptr)
	{
		dispatcher.register_handler(handler, MASK_READ | MASK_WRITE);
	}

	_BLOCK_END_

	return ret;
}

int EventHandler::handle_event(EventDispatcher &dispatcher, const int mask)
{
	int ret = 0;
	const int fd = _handle->get_handle();

	_BLOCK_START_

	if(mask & MASK_READ)
	{
		ret = _handle->read(_ibuf);
		BREAK_FAILED(ret == 0, NULL);
		ret = handle_input(_ibuf);
		BREAK_FAILED(ret == 0, NULL);

		//TODO: 如果是客户端需要考虑是否关闭socket，发生错误时必须关闭
	}
	else if(mask & MASK_WRITE)
	{
		ret = handle_output(_obuf);
		BREAK_FAILED(ret == 0, NULL);
		ret = _handle->write(_obuf);
		BREAK_FAILED(ret == 0, NULL);

		//TODO: 如果是服务端需要考虑是否关闭socket，发生错误时必须关闭
	}

	_BLOCK_END_

	return ret;
}

}; // namespace ec
