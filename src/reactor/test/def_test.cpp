#include <iostream>
using namespace std;

#ifdef __APPLE__
	#include <AvailabilityMacros.h>
#endif

#ifdef __linux__
	#define HAVE_EPOLL 1
	#define IS_LINUX 1
#endif

#if (defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6)) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined (__NetBSD__)
	#define HAVE_KQUEUE 1
	#define IS_MACOS 1
#endif

#ifdef __sun
	#include <sys/feature_tests.h>
	#ifdef _DTRACE_VERSION
		#define HAVE_EVPORT 1
		#define IS_SUN 1
	#endif
#endif

#if (defined(_WIN32) || defined(_MSC_VER)) // TODO
	#define HAVE_IOCTL 1	
	#define IS_WINDOWS 1	
#endif
/*
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
        //#include "select_poller.h"
		//typedef SelectPoller EventPollerImpl;
        #endif
    #endif
#endif
*/
int main()
{
	#if defined(HAVE_EVPORT)
		cout << "HAVE_EVPORT: " << HAVE_EVPORT << ", IS_SUN: " << IS_SUN << endl;
	#elif defined(HAVE_EPOLL)
		cout << "HAVE_EPOLL: " << HAVE_EPOLL << ", IS_LINUX: " << IS_LINUX << endl;
	#elif defined(HAVE_KQUEUE)
		cout << "HAVE_KQUEUE: " << HAVE_KQUEUE << ", IS_MACOS: " << IS_MACOS << endl;
	#else
		cout << "defined none" << endl;
	#endif
}
