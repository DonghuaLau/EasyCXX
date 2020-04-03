#ifndef _EC_DISPATCHER_H_
#define _EC_DISPATCHER_H_

#include "arch.h"
#include "poller.h"
#include "handler.h"

using namespace std;

namespace ec
{

struct EventItem
{
	int fd;
	int mask;
	shared_ptr<EventHandler> *event_handler;

	void init()
	{
		fd = 0;
		mask = MASK_NONE;
		event_handler.reset();
	}
};

class EventDispatcher : public CProducer<EventItem>
{
private:
	shared_ptr<EventPoller> _poller;
	map<int, shared_ptr<EventHandler> > _handlers;
	int _event_size = 64;

	class EventProducer;
	class EventConsumer;

	int _consumer_thd_num = 2;
	CRepository<EventItem> & _repo; 
	vector<shared_ptr<EventConsumer> > _consumer_list;
	shared_ptr<EventProducer> _producer;

protected:
	friend EventPollerImpl;
	int event_fired(EventItem &item);	

public:
	EventDispatcher(int event_size, int consumer_thd_num = 2);
	~EventDispatcher();

	int handle_events();
	int register_handler(shared_ptr<EventHandler> & handler, const int mask);
	int remove_handler(shared_ptr<EventHandler> & handler, const int mask);
	inline void wait();

private:
	class EventProducer : public CProducer<EventItem>
	{
	private:
		EventDispatcher &_event_dispatcher;
	public:
		EventProducer(CRepository<EventItem> &repo, EventDispatcher &event_dispatcher);
		inline virtual int produce();
	};

	class EventConsumer : public CConsumer<EventItem>
	{
	public:
		EventConsumer(CRepository<EventItem> & repo);
		inline virtual int consume(EventItem &item);
	};
};

}; // namespace ec

#endif

