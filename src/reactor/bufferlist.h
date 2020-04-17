#ifndef _EC_BUFFERLIST_H_
#define _EC_BUFFERLIST_H_

#include <list>
using namespace std;

namespace ec
{

typedef unsigned char byte;

class Buffer
{
private:
	byte *_buf = nullptr;
	int _len = 0;
	int _capacity = 64; // no basis

public:
	Buffer()
	{
		_buf = new byte[_capacity];
	}

	Buffer(int capacity):_capacity(capacity)
	{
		_buf = new byte[_capacity];
	}

	~Buffer()
	{
		if(_capacity > 0)
		{
			delete[] _buf;
			_capacity = 0;
			_len = 0;
		}
	}

	void *get_ptr()
	{
		return _buf;
	}

	void *get_ptr(const int capacity, int pos = 0)
	{
		if(pos < 0 || capacity <= 0 || _capacity - pos < capacity) return nullptr;
		else return _buf + pos;
	}

	void set_length(const int len, const bool is_append = false)
	{
		if(is_append == true) _len += len;
		else _len = len;
	}

	int get_length()
	{
		return _len;
	}

	int get_capacity()
	{
		return _capacity;
	}
};

class BufferList
{
private:
	int _size = 0;
	list<shared_ptr<Buffer> > _buf_list; // 采用指针避免多次拷贝

public:
	BufferList(){}
	~BufferList(){}

	void append(shared_ptr<Buffer> &buffer)
	{
		_buf_list.push_back(buffer); 
		_size += buffer->get_length();
	}

	int get_size()
	{
		return _size;
	}

	list<shared_ptr<Buffer> > & get_list()
	{
		return _buf_list;
	}
};

}; // namespace ec

#endif



