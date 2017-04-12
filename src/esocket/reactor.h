#ifndef _EC_REACTOR_H_
#define _EC_REACTOR_H_

#include "esocket.h"

namespace easycxx
{

class Reactor
{
public:
	Reactor();
	~Reactor();

	int add(std::string remote_host, int remote_port, int is_server = 0);
	int del(std::string remote_host, int remote_port, int is_server = 0);

	int handle_timeout();

	int poll();

private:

	EventModel *_event_model;
};


}

#endif

