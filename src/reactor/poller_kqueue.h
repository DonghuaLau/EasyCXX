#ifndef _EC_KQUEUE_POLLER_H_
#define _EC_KQUEUE_POLLER_H_

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include "poller_base.h"

namespace ec
{

class EventDispatcher;

class KQueuePoller : public EventPoller
{
private:
    int _kqfd = 0;
    //shared_ptr<struct kevent> _events; // C++11的shared_ptr不支持数组
    struct kevent *_events;
	EventDispatcher &_event_dispatcher;

public:
	KQueuePoller(int setsize, EventDispatcher &dispatcher);
	virtual ~KQueuePoller();

	virtual int create();
	virtual int add_event(const int fd, const int mask);
	virtual int del_event(const int fd, const int mask);
	virtual int event_poll(const struct timeval *tvp);
	virtual int resize(const int setsize);
	virtual int destory();
	virtual string &get_name();
};

}; // namespace ec

#endif

