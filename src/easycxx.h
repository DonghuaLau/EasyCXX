#ifndef _EASYCXX_H_
#define _EASYCXX_H_

#define ECERRORS
#ifdef ECERRORS
	#include <errno.h> 
	#include "common/errors.h"
#endif

//#define ECBUFFER
#ifdef ECBUFFER
	#include "common/byte_buffer.h"
#endif

#ifdef MEMORY_POOL
	#include "common/memory_pool.h"
#endif

#ifdef ESOCKET
	#include "esocket/esocket.h"
#endif

#endif
