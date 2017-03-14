#ifndef _EC_ERRORS_H_
#define _EC_ERRORS_H_

namespace easycxx
{

enum ECError
{
	 OK 		= 0
	,SUCCESS 	= 0
	,ERROR 		= -1
	,FAILED 	= -1
	// > 10000, errno compatible

	// socket errors: esocket/esocket_errors.h
	
	// buffer errors
	,BUFFER_NOT_ENOUGH		= 20001

};

}

#endif
