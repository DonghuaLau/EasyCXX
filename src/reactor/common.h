#ifndef _EC_REACTOR_COMMON_H_
#define _EC_REACTOR_COMMON_H_

namespace ec
{

#define _BLOCK_START_ do{
#define _BLOCK_END_   }while(0);

#define BREAK_FAILED(cond, expr) if(expr == false){ expr; break; }
#define RETURN_FAILED(cond, err) if(expr == false){ return err; }

// log
#define dlog cout
#define ilog cout
#define wlog cerr
#define elog cerr

}; // namespace ec

#endif

