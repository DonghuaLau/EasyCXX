#ifndef _EC_HANDLE_H_
#define _EC_HANDLE_H_

#include "common.h"
#include "bufferlist.h"

using namespace std;

namespace ec
{

class Handle
{
public:
	Handle(){}
	virtual ~Handle(){}
	virtual int get_handle() = 0;
	//virtual int set_handle(int fd) = 0;
	virtual int listen(const string &ip, const int port) = 0;
	virtual int accept(shared_ptr<Handle> &client_handle) = 0;
	virtual int read(shared_ptr<BufferList> &buf) = 0;
	virtual int write(shared_ptr<BufferList> &buf) = 0;
	virtual int set_nonblock(int fd) = 0;
};

//TODO: class FileHandle;
//TODO: class TimerHandle;
//TODO: UDPHandle
class TCPHandle : public Handle
{
protected:
	int _fd;
	int _port;
	string _ip;
	int _ip_version; // EN_IP_VERSION
	int _read_size = READ_BUF_SIZE;

public:
	TCPHandle();
	virtual ~TCPHandle();

	virtual int get_handle();
	virtual void set_read_size(const int read_size);
	virtual int listen(const string &ip, const int port);
	virtual int accept(shared_ptr<Handle> &client_handle);
	virtual int read(shared_ptr<BufferList> &buf);
	virtual int write(shared_ptr<BufferList> &buf);

private:
	virtual int set_nonblock(int fd);

};

}; // namespace ec

#endif

