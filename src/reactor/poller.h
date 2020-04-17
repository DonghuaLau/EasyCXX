#ifndef _EC_POLLER_H_
#define _EC_POLLER_H_

//#include "common.h"
#include "poller_base.h"

#ifdef HAVE_EVPORT
	//#include "evport_poller.h"
#else
    #ifdef HAVE_EPOLL
    	#include "poller_epoll.h"
		namespace ec
		{
			//typedef EPollPoller EventPollerImpl;
			using EPollPoller EventPollerImpl;
		}; // namespace ec
    #else
        #ifdef HAVE_KQUEUE
        #include "poller_kqueue.h"
		namespace ec
		{
			//typedef KQueuePoller EventPollerImpl;
			using KQueuePoller EventPollerImpl;
		}; // namespace ec
        #else
        //#include "poller_select.h"
		//typedef SelectPoller EventPollerImpl;
        #endif
    #endif
#endif

#endif

