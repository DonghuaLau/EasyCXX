#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include "memory_pool.h"
#include "define.h"

namespace easycxx
{

static int g_default_length = -1;

class EByteBuffer
{
private:
	byte *_buffer;
	int _buffer_size;
	int _end_pos; // next pos of last byte

public:
	EByteBuffer();
	EByteBuffer(int size);
	//EByteBuffer(int size, int value);
	~EByteBuffer();

	const byte *get_buffer(const int start = 0, int &length = g_default_length) const; // length -1 means all bytes
	int push_back(const byte *buffer, const int length);
	const int size(); // Why can't declare inline 
	const int get_remain_size();
	const int capacity();

};

}

#endif
