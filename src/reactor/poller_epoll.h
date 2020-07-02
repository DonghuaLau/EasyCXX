#ifndef _EC_EPOLL_POLLER_H_
#define _EC_EPOLL_POLLER_H_

#include "poller_base.h"

namespace ec
{

class EPollPoller : public EventPoller
{
private:
	EventDispatcher &_event_dispatcher;

public:
	KQueuePoller(EventDispatcher &dispatcher, int setsize);

public:
	EPollPoller(EventDispatcher &dispatcher, int setsize)
		:EventPoller(setsize),_event_dispatcher(dispatcher){}
	virtual ~EPollPoller()
	{
	}

	virtual int create();
	virtual int add_event(int fd, int mask);
	virtual int del_event(int fd, int mask);
	virtual int event_poll(struct timeval &timeout);
	virtual int destory();
	virtual int resize(int setsize);
	virtual string &get_name();

};

}; // namespace ec

#endif

