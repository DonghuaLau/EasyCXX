#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

namespace easycxx
{

class EByteBuffer
{
private:
	byte *_buffer;
	int _buffer_size;
	int _end_pos;

public:
	EByteBuffer();
	EByteBuffer(int size);
	//EByteBuffer(int size, int value);
	~EByteBuffer();

	byte *get_buffer(int start = 0, int length = -1); // length -1 means all bytes
	int push_back(byte *buffer, int length);

};

}

#endif
