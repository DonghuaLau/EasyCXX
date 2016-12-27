#ifndef _EASYCXX_H_
#define _EASYCXX_H_

#define ECERRORS
#ifdef ECERRORS
	#include <errno.h> 
	#include "common/errors.h"
#endif

#ifdef MEMORY_POOL
	#include "common/memory_pool.h"
#endif

#ifdef ESOCKET
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include "esocket/socket.h"
	#include "esocket/tcp_socket.h"
	#include "esocket/udp_socket.h"
#endif

#endif
