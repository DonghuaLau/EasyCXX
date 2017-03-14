#include <iostream>
#include <sstream>
#include <cstring>
#include <string.h>
#include "common_test.h"
#include "define.h"
#include "byte_buffer.h"

void test_1()
{
	easycxx::EByteBuffer buf;

	char str[] = "Hello world, ";
	int len = buf.push_back((easycxx::byte*)str, strlen(str));
	std::cout << "pushed length: " << len << std::endl;
	std::cout << "get buffer 1: " << (char*)buf.get_buffer() << ", length: " << len << std::endl;
	std::cout << "get buffer 5: " << (char*)buf.get_buffer(5, len) << ", length: " << len << std::endl;
	std::cout << "length: " << len << std::endl << std::endl;

	char str2[] = "I'm C++ framework.";
	int len2 = buf.push_back((easycxx::byte*)str2, strlen(str2));
	std::cout << "pushed length: " << len2 << std::endl;
	std::cout << "get buffer 1: " << (char*)buf.get_buffer() << ", length: " << buf.size() << std::endl;
	len2 = 8;
	std::cout << "get buffer 5: " << (char*)buf.get_buffer(5, len2) << ", length: " << len2 << std::endl;
	std::cout << "length: " << len2 << std::endl << std::endl;

	len2 = 8;
	std::cout << "get buffer 100: " << (char*)buf.get_buffer(100, len2) << ", length: " << len2 << std::endl;
	std::cout << "length: " << len2 << std::endl << std::endl;
}

void test_2()
{
}

int main()
{
	test_1();
	test_2();
	return 0;
}


