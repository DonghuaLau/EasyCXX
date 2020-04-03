#ifndef _EC_POLLER_H_
#define _EC_POLLER_H_

#include "arch.h"

namespace ec
{

class EventPoller
{
private:
	int _setsize = 64;

public:
	EventPoller(int setsize):_setsize(setsize){}
	virtual ~EventPoller()
	{
		destory(); // need call in sub class?
	}

	virtual int create() = 0;
	virtual int add_event(const int fd, const int mask) = 0;
	virtual int del_event(const int fd, const int mask) = 0;
	virtual int event_poll(const struct timeval &timeout) = 0;
	virtual int resize(const int setsize)  = 0;
	virtual int destory() = 0;
	virtual string &get_name() = 0;

	void set_size(const int setsize)
	{
		_setsize = setsize;
	}

	int get_size() const
	{
		return _setsize;
	}
};

}; // namespace ec

#ifdef HAVE_EVPORT
	//#include "evport_poller.h"
#else
    #ifdef HAVE_EPOLL
    	#include "epoll_poller.h"
		typedef EPollPoller EventPollerImpl;
    #else
        #ifdef HAVE_KQUEUE
        #include "kqueue_poller.h"
		typedef KQueuePoller EventPollerImpl;
        #else
        #include "select_poller.h"
		typedef SelectPoller EventPollerImpl;
        #endif
    #endif
#endif

#endif

