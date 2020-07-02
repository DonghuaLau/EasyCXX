
#include <memory>
#include "dispatcher.h"

namespace ec
{

EventDispatcher::EventDispatcher(int event_size, int consumer_thd_num/* = 2*/)
		:_repo(event_size)
		,_event_size(event_size)
		,_consumer_thd_num(consumer_thd_num)
{
	_poller = std::make_shared<EventPollerImpl>(_event_size, *this);
	_poller->create();

	_producer = std::make_shared<EventProducer>(_repo, *this);
	_producer->start();
	for(int i = 0; i < _consumer_thd_num; i++)
	{
		_consumer_list[i] = std::make_shared<EventConsumer>(_repo, *this);
		_consumer_list[i]->start();
	}
}

EventDispatcher::~EventDispatcher()
{
}

inline int EventDispatcher::handle_events()
{
	struct timeval tval;
	tval.tv_sec = 5;
	tval.tv_usec = 0;

	while(true)
	{
		_poller->event_poll(&tval);
	}
	return 0;
}

inline bool EventDispatcher::is_server_handle(const int fd)
{
	map<int, shared_ptr<EventHandler> >::iterator it = _svr_handlers.find(fd);
	return it != _svr_handlers.end();
}

int EventDispatcher::event_fired(EventItem &item)
{
	RETURN_FAILED(item.fd > 0, EventError::E_INVALID_VAL);

	map<int, shared_ptr<EventHandler> >::iterator it = _svr_handlers.find(item.fd);
	if((item.mask & MASK_READ) && it != _svr_handlers.end())
	{
		item.is_listened = true;
		item.event_handler = it->second;
		_producer->produce(item);
	}
	else
	{
		it = _client_handlers.find(item.fd);
		if(it != _client_handlers.end())
		{
			item.event_handler = it->second;
			_producer->produce(item);
		}
	}
	return 0;
}

inline int EventDispatcher::register_server(shared_ptr<EventHandler> & handler)
{
	int ret = 0;
	int mask = MASK_READ | MASK_WRITE;
	ret = add_handler(handler, mask, _svr_handlers);
	return ret;
}

inline int EventDispatcher::register_handler(shared_ptr<EventHandler> & handler, const int mask)
{
	int ret = 0;
	ret = add_handler(handler, mask, _client_handlers);
	return ret;
}

int EventDispatcher::add_handler(shared_ptr<EventHandler> & handler, const int mask
								, map<int, shared_ptr<EventHandler> > &map_handlers)
{
	int ret = 0;
	const int fd = handler->get_handle();
	map<int, shared_ptr<EventHandler> >::iterator it = map_handlers.find(fd);
	if(it == map_handlers.end())
	{
		ret = _poller->add_event(fd, mask);
		RETURN_FAILED(ret == 0, ret);
		map_handlers.insert(pair<int, shared_ptr<EventHandler> >(fd, handler));
	}
	else
	{
		dlog << "fd[" << fd << "] has exist and ignore it" << endl;
	}
	return ret;
}

int EventDispatcher::remove_handler(shared_ptr<EventHandler> & handler, const int mask)
{
	int ret = 0;
	const int fd = handler->get_handle();
	map<int, shared_ptr<EventHandler> >::iterator it = _client_handlers.find(fd);
	if(it != _client_handlers.end())
	{
		ret = _poller->del_event(fd, mask);
		RETURN_FAILED(ret == 0, ret);
		_client_handlers.erase(fd);
	}
	else
	{
		dlog << "fd[" << fd << "] not found and ignore it" << endl;
	}
	return ret;
}

inline void EventDispatcher::wait()
{
	_producer->join();
	for(int i = 0; i < _consumer_thd_num; i++)
	{
		_consumer_list[i]->join();
	}
}

EventDispatcher::EventProducer::EventProducer(CRepository<EventItem> &repo, EventDispatcher &event_dispatcher)
	:CProducer<EventItem>(repo)
	,_event_dispatcher(event_dispatcher)
{
}

inline int EventDispatcher::EventProducer::produce()
{
	_event_dispatcher.handle_events();
	return 0;
}

inline int EventDispatcher::EventProducer::produce(EventItem &item) 
{
	CProducer<EventItem>::produce(item);
	return 0;
}

EventDispatcher::EventConsumer::EventConsumer(CRepository<EventItem> & repo, EventDispatcher &event_dispatcher)
	:CConsumer<EventItem>(repo)
	,_event_dispatcher(event_dispatcher)
{
}

inline int EventDispatcher::EventConsumer::consume(EventItem &item)
{
	int ret = 0;
	if(item.event_handler.get() != nullptr)
	{
		if(item.is_listened)
		{
			ret = item.event_handler->handle_accept(_event_dispatcher, item.mask);
		}
		else
		{
			ret = item.event_handler->handle_event(_event_dispatcher, item.mask);
		}
	}
	else
	{
		wlog << "wired, fd: " << item.fd << " with null event_handler" << endl;
	}
	return ret;
}

}; // namespace ec
