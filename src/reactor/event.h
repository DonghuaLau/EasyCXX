#ifndef _EC_EVENT_H_
#define _EC_EVENT_H_

#include "common.h"
#include "dispatcher.h"

namespace ec
{

// common define

enum EN_EVENT_STATUS
{
	STATUS_NONE		= 0,
	STATUS_RUNNING	= 1,
	STATUS_STOPPED	= 2,
	STATUS_CLOSED	= 3,
};

enum EN_EVENT_MASK
{
	MASK_NONE	= 0b00000,
	MASK_READ	= 0b00001,
	MASK_WRITE	= 0b00010,
};

}; // namespace ec

#endif

