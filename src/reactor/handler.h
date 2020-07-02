#ifndef _EC_HANDLER_H_
#define _EC_HANDLER_H_

#include "common.h"
#include "bufferlist.h"
#include "handle.h"
//#include "dispatcher.h"

using namespace std;

namespace ec
{

class EventDispatcher;

class EventHandler
{
private:
	shared_ptr<Handle> _handle;
	//int _status;
	int _link_type = LINK_SHORT;
	int _event_type = EVENT_TCP; // unnecessary?  tcp, udp, unix socket, file ...
	bool _is_svr_handle = false;

protected:
	shared_ptr<BufferList> _ibuf; // for input
	shared_ptr<BufferList> _obuf; // for output
	
public:
	EventHandler(bool is_svr_handle = false);
	//EventHandler(shared_ptr<Handle> &handle);
	virtual ~EventHandler();

	void set_handle(shared_ptr<Handle> &handle);
	int get_handle();
	void set_link_type(const int link_type);
	int get_link_type();
	void set_event_type(const int event_type);
	int get_event_type();

	virtual int server_init(const string &ip, const int port);

	// create Handler if there's a new connection for server
	virtual shared_ptr<EventHandler> create_handler(shared_ptr<Handle> &handle) = 0; 

	virtual int handle_accept(EventDispatcher &dispatcher, const int mask);
	virtual int handle_event(EventDispatcher &dispatcher, const int mask);
	virtual int handle_input(shared_ptr<BufferList> &ibuf) = 0;
	virtual int handle_output(shared_ptr<BufferList> &obuf) = 0;
};

}; // namespace ec

#endif

