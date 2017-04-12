#ifndef _EC_EPOLL_EVENT_MODAL_H_
#define _EC_EPOLL_EVENT_MODAL_H_

#include "event_model.h"

namespace easycxx
{

class EpollEventModel : public EventModel
{
public:
	EpollEventModel();
	virtual ~EpollEventModel();

	virtual bool init(int max_events = 1024, bool is_server = false);
	virtual int add(const Connection &conn);
	virtual int del(const Connection &conn);
	virtual int modify(const Connection &conn);
	virtual int poll();
	virtual int select(int timeout = 10);
	virtual int get_last_error();

private:

	int _epoll_fd;
	int _max_events;
	struct epoll_event *_events;

	int _last_error;

	ConnectionMap *_conn_map;
	
	
};

}

#endif


