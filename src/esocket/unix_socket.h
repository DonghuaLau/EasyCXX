#ifndef _EC_UNIX_SOCKET_H_
#define _EC_UNIX_SOCKET_H_

namespace easycxx
{

class EUnixSocket
{
private:
	int _socket;

public:
	EUnixSocket(){}
	virtual ~EUnixSocket(){}
};

}

#endif


