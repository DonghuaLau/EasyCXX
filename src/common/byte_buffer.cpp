#include <iostream>
#include <string.h>
#include "byte_buffer.h"

namespace easycxx
{

EByteBuffer::EByteBuffer()
	:_buffer_size(1024), _end_pos(0)
{
	_buffer = new byte[_buffer_size]; // default 1024 btyes
	std::cout << "[" << __func__ << "] buffer size: " << _buffer_size << ", end pos: " << _end_pos << std::endl;
}

EByteBuffer::EByteBuffer(const int size)
	:_buffer_size(size), _end_pos(0)
{
	_buffer = new byte[_buffer_size];
}

EByteBuffer::~EByteBuffer()
{
	delete[] _buffer;
}

// length -1 means all bytes
const byte *EByteBuffer::get_buffer(const int start, int &length) const
{
	std::cout << "[" << __func__ << "] start pos: " << start << ", end pos: " << _end_pos << ", length: " << length << std::endl;
	if(start >= _end_pos){ return NULL; }
	if(length == -1){ length = _end_pos+1; }
	if(start + length > _end_pos){ length = _end_pos - start; }
	return _buffer + start;
}

// return pushed length
int EByteBuffer::push_back(const byte *buffer, const int length)
{
	int push_length = length < (_buffer_size - _end_pos) ? length : (_buffer_size - _end_pos);
	
	std::cout << "[" << __func__ << "] push length: " << push_length << ", end pos: " << _end_pos << std::endl;
	if(push_length <= 0) return -1; // buffer overflow 

	memcpy(_buffer + _end_pos, buffer, push_length);
	_end_pos += push_length;
	std::cout << "[" << __func__ << "] push length: " << push_length << ", end pos: " << _end_pos << std::endl;

	return push_length; 
}

const int EByteBuffer::size()
{
	return _end_pos;
}

const int EByteBuffer::get_remain_size()
{
	return _buffer_size - _end_pos;
}

const int EByteBuffer::capacity()
{
	return _buffer_size;
}

void EByteBuffer::clear()
{
	_end_pos = 0;
}

}
