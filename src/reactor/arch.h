#ifndef _EC_ARCH_H_
#define _EC_ARCH_H_

/*
#define IS_X86
#define IS_X64

#define IS_WINDOWS // ioctlsocket
#define IS_LINUX
#define IS_MACOS
#define IS_SUN
*/

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

#ifdef _WIN32 || _MSC_VER // TODO
	#define HAVE_IOCTL 1	
	#define IS_WINDOWS 1	
#endif

#endif
