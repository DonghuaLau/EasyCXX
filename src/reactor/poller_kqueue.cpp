#include <unistd.h>
#include "common.h"
#include "poller_kqueue.h"
#include "dispatcher.h"

namespace ec
{

KQueuePoller::KQueuePoller(int setsize, EventDispatcher &dispatcher)
	:EventPoller(setsize)
	,_event_dispatcher(dispatcher)
{
}

KQueuePoller::~KQueuePoller()
{
	destory();
}

int KQueuePoller::create()
{
	int ret = 0;

	_BLOCK_START_

	BREAK_FAILED(get_size() > 0, ret = EventError::E_INVALID_PARAM);

	_events = new struct kevent[get_size()];
	BREAK_FAILED(_events != nullptr, ret = EventError::E_ALLOCATE);

    _kqfd = kqueue();
	BREAK_FAILED(_kqfd != -1, ret = EventError::E_ALLOCATE);

	_BLOCK_END_

	return ret;
}

inline int KQueuePoller::add_event(const int fd, const int mask)
{
    struct kevent ke;

    if(mask & MASK_READ)
	{
        EV_SET(&ke, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        if (kevent(_kqfd, &ke, 1, NULL, 0, NULL) == -1) 
		{
			return EventError::E_ADD_EVENT;
		}
    }

    if (mask & MASK_WRITE) {
        EV_SET(&ke, fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
        if (kevent(_kqfd, &ke, 1, NULL, 0, NULL) == -1)
		{
			return EventError::E_ADD_EVENT;
		}
    }

	return 0;
}

inline int KQueuePoller::del_event(const int fd, const int mask)
{
    struct kevent ke;

    if (mask & MASK_READ) {
        EV_SET(&ke, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        kevent(_kqfd, &ke, 1, NULL, 0, NULL);
    }
    if (mask & MASK_WRITE) {
        EV_SET(&ke, fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
        kevent(_kqfd, &ke, 1, NULL, 0, NULL);
    }

	return 0;
}

int KQueuePoller::event_poll(const struct timeval *tvp)
{
	int ret = 0;
    int numevents = 0;

    if (tvp != NULL) 
	{
        struct timespec timeout;
        timeout.tv_sec = tvp->tv_sec;
        timeout.tv_nsec = tvp->tv_usec * 1000;
        ret = kevent(_kqfd, NULL, 0, _events, get_size(), &timeout);
    } 
	else 
	{
        ret = kevent(_kqfd, NULL, 0, _events, get_size(), NULL);
    }

    if (ret > 0) 
	{
    	EventItem item;
		numevents = ret;
		struct kevent *e = NULL;

		for(int j = 0; j < numevents; j++) 
		{
			e = _events+j;
			item.fd = e->ident;
			if (e->filter == EVFILT_READ) item.mask |= MASK_READ;
			if (e->filter == EVFILT_WRITE) item.mask |= MASK_WRITE;
			_event_dispatcher.event_fired(item);
		}
    }

    return numevents;
}

inline int KQueuePoller::resize(const int setsize)
{
	int ret = 0;

	_BLOCK_START_

	BREAK_FAILED(setsize > 0, ret = EventError::E_INVALID_PARAM);

	set_size(setsize);
	delete[] _events;
	_events = new struct kevent[get_size()];
	BREAK_FAILED(_events != nullptr, ret = EventError::E_ALLOCATE);
	
	_BLOCK_END_

	return ret;
}

inline int KQueuePoller::destory()
{
	if(_kqfd > 0)
	{
    	::close(_kqfd);
		_kqfd = 0;
	}

	if(_events != nullptr)
	{
		delete[] _events;
	}

	return 0;
}

inline string &KQueuePoller::get_name()
{
	static string name = "kqueue";
	return name;
}

}; // namespace ec
