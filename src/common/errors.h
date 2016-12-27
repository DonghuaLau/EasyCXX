#ifndef _EC_ERRORS_H_
#define _EC_ERRORS_H_

namespace easycxx
{

enum ECError
{
	 OK 		= 0
	,SUCCESS 	= 0
	,ERROR 		= 1
	,FAILED 	= 1
	// > 10000, errno compatible

	// socket errors
	,SOCK_ERR_CONNECT 		= 10001
	,SOCK_ERR_INET_PTON 	= 10002
};

#endif
