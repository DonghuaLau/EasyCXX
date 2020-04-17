#ifndef _EC_REACTOR_COMMON_H_
#define _EC_REACTOR_COMMON_H_

#include <string>
#include "arch.h"
#include "error.h"

using namespace std;

namespace ec
{

#define _BLOCK_START_ do{
#define _BLOCK_END_   }while(0);

#define BREAK_FAILED(cond, expr) if(cond == false){ expr; break; }
#define RETURN_FAILED(cond, err) if(cond == false){ return err; }

// log
#define dlog cout
#define ilog cout
#define wlog cerr
#define elog cerr

#define NET_IP_STR_LEN 46
#define READ_BUF_SIZE 512
#define DEFAULT_TCP_BACKLOG 511 // why redis set default 511

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

enum EN_EVENT_TYPE
{
	EVENT_NONE	= 0,
	EVENT_TCP	= 1,
	EVENT_UDP	= 2,
	EVENT_TIMER	= 3,
	EVENT_FILE	= 4,
};

enum EN_LINK_TYPE
{
	LINK_NONE	= 0,
	LINK_SHORT	= 1,
	LINK_LONG	= 2,
};

enum EN_IP_VERSION
{
	IPV4	= 4,
	IPV6	= 6,
};


}; // namespace ec

#endif

