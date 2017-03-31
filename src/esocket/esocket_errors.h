#ifndef _ESOCKET_ERRORS_H_
#define _ESOCKET_ERRORS_H_

namespace easycxx
{

enum SocketError
{
	 SCOK_OK				= 2000

	,SOCK_ERR_CONNECT 		= 2001
	,SOCK_ERR_INET_PTON 	= 2002

	,SCOK_ERR_END			= 2999

};

}

#endif

