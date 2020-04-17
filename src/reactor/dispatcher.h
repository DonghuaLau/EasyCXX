#ifndef _EC_DISPATCHER_H_
#define _EC_DISPATCHER_H_

#include "common.h"
#include "producer_consumer.h"
#include "poller.h"
#include "handler.h"

using namespace std;

namespace ec
{

struct EventItem
{
	int fd;
	int mask;
	bool is_listened;
	shared_ptr<EventHandler> event_handler;

	EventItem()
	{
		init();
	}

	void init()
	{
		fd = 0;
		mask = MASK_NONE;
		is_listened = false;
		event_handler.reset();
	}
};

class EventDispatcher
{
private:
	shared_ptr<EventPoller> _poller; // (a poller only support a listened socket?)
	map<int, shared_ptr<EventHandler> > _svr_handlers; // support multi svr 
	map<int, shared_ptr<EventHandler> > _client_handlers; //_handlers;
	int _event_size = 64;

	class EventProducer;
	class EventConsumer;

	int _consumer_thd_num = 2;
	CRepository<EventItem> _repo; 
	vector<shared_ptr<EventConsumer> > _consumer_list;
	shared_ptr<EventProducer> _producer;

protected:
	friend EventPollerImpl;
	int event_fired(EventItem &item);	

public:
	EventDispatcher(int event_size, int consumer_thd_num = 2);
	~EventDispatcher();

	int handle_events();
	int register_server(shared_ptr<EventHandler> & handler);
	int register_handler(shared_ptr<EventHandler> & handler, const int mask);
	int remove_handler(shared_ptr<EventHandler> & handler, const int mask);
	bool is_server_handle(const int fd);	
	void wait();

private:
	int add_handler(shared_ptr<EventHandler> & handler, const int mask
					, map<int, shared_ptr<EventHandler> > &map_handlers);

	class EventProducer : public CProducer<EventItem>
	{
	private:
		EventDispatcher &_event_dispatcher;
	public:
		EventProducer(CRepository<EventItem> &repo, EventDispatcher &event_dispatcher);
		virtual int produce();
		int produce(EventItem &item);
	};

	class EventConsumer : public CConsumer<EventItem>
	{
	public:
		EventDispatcher &_event_dispatcher;
		EventConsumer(CRepository<EventItem> & repo, EventDispatcher &event_dispatcher);
		virtual int consume(EventItem &item);
	};
};

}; // namespace ec

#endif

