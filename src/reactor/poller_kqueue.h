#ifndef _EC_KQUEUE_POLLER_H_
#define _EC_KQUEUE_POLLER_H_

#include <sys/types.h>
#include <sys/event.h>
//#include <sys/time.h>

#include "poller.h"

namespace ec
{

class KQueuePoller
{
private:
    int _kqfd = 0;
    shared_ptr<struct kevent> _events;
	EventDispatcher &_event_dispatcher;

public:
	KQueuePoller(EventDispatcher &dispatcher, int setsize);
	virtual ~KQueuePoller();

	inline virtual int create();
	virtual int add_event(const int fd, const int mask);
	virtual int del_event(const int fd, const int mask);
	virtual int event_poll(const struct timeval &timeout);
	inline virtual int resize(const int setsize);
	inline virtual int destory();
	inline virtual string &get_name();
};

}; // namespace ec

#endif

