#ifndef _ESOCKET_ERRORS_H_
#define _ESOCKET_ERRORS_H_

namespace easycxx
{

enum SocketError
{
	// common errors
	 COM_OK					= 1000
	,ERR_NULL				= 1001


	// socket errors
	,SCOK_OK				= 2000

	,SOCK_ERR_CONNECT 		= 2001
	,SOCK_ERR_INET_PTON 	= 2002
	,SOCK_ERR_NULL			= 2003
	,SOCK_ERR_NOT_OPEN		= 2004

	,SCOK_ERR_END			= 2999

};

}

#endif

