#include "dispatcher.h"

EventDispatcher::EventDispatcher(int event_size, int consumer_thd_num/* = 2*/)
		:_event_size(event_size), _consumer_thd_num(consumer_thd_num)
{
	_poller = std::make_shared<EventPollerImpl>(_event_size);
	_poller->create();

	_producer = std::make_shared<EventProducer>(_repo, *this);
	_producer->start();
	for(int i = 0; i < _consumer_thd_num; i++)
	{
		_consumer_list[i] = std::make_shared<EventConsumer>(_repo);
		_consumer_list[i]->start();
	}
}

EventDispatcher::~EventDispatcher()
{
}

int EventDispatcher::handle_events()
{
	while(true)
	{
		_poller->event_poll();
	}
	return 0;
}

int EventDispatcher::event_fired(EventItem &item)
{
	RETURN_FAILED(item.fd > 0, E_INVALID_VAL);

	map<int, shared_ptr<EventHandler> >::iterator it = _handlers.find(item.fd);
	if(it != _handlers.end())
	{
		item.event_handler = it->second;
		CProducer<EventItem>::produce(item);
	}
	return 0;
}

int EventDispatcher::register_handler(shared_ptr<EventHandler> & handler, const int mask)
{
	int ret = 0;
	const int fd = handler->get_handle();
	map<int, shared_ptr<EventHandler> >::iterator it = _handlers.find(fd);
	if(it == _handlers.end())
	{
		ret = _poller.add_event(fd, mask);
		BREAK_FAILED(ret == 0, NULL);
		_handlers.insert(pair<int, shared_ptr<EventHandler> >(fd, handler));
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
	map<int, shared_ptr<EventHandler> >::iterator it = _handlers.find(fd);
	if(it != _handlers.end())
	{
		ret = _poller.del_event(fd, mask);
		BREAK_FAILED(ret == 0, NULL);
		_handlers.erase(fd);
	}
	else
	{
		dlog << "fd[" << fd << "] not found and ignore it" << endl;
	}
	return ret;
}

void EventDispatcher::wait()
{
	_producer.join();
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

int EventDispatcher::EventProducer::produce()
{
	_event_dispatcher->handle_events();
	return 0;
}

EventDispatcher::EventConsumer::EventConsumer(CRepository<EventItem> & repo)
	:CConsumer<EventItem>(repo)
{
}

int EventDispatcher::EventConsumer::consume(EventItem &item)
{
	int ret = 0;
	if(item->event_handler->get() != nullptr)
	{
		ret = item->event_handler->handle_event(item->_mask);
	}
	else
	{
		wlog << "wired, fd: " << itme.fd << " with null event_handler" << endl;
	}
	return ret;
}
