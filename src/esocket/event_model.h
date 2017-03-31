#ifndef _EC_EVENT_MODAL_H_
#define _EC_EVENT_MODAL_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

namespace easycxx
{

class EventModel
{
public:
	EventModel(){}
	virtual ~EventModel(){}

	virtual bool init(int max_events = 1024, bool is_server = false) = 0;
};

}

#endif

