#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <errno.h>
#include "byte_buffer.h"
//#include "esocket.h"
#include "tcp_socket.h"

// test tcp socket 
void tcp_socket_test()
{
	const unsigned char text[] = "Hello World!";
	int len = strlen((const char*)text);
	easycxx::EByteBuffer buffer;
	buffer.push_back(text, len);

	easycxx::TCPSocket tcp_sock;
	int ret = tcp_sock.connect(std::string("127.0.0.1"), 1800);
	std::cout << "connect ret: " << ret << std::endl;
	ret = tcp_sock.send(buffer);
	std::cout << "send ret: " << ret << std::endl;
}

// test tcp socket 
void tcp_socket_test_2()
{
	const unsigned char text[] = "Hello World 2!";
	int len = strlen((const char*)text);
	easycxx::EByteBuffer buffer;
	buffer.push_back(text, len);

	//easycxx::TCPSocket tcp_sock();
	easycxx::TCPSocket *tcp_sock;
	tcp_sock = new easycxx::TCPSocket();
	int ret = tcp_sock->connect(std::string("127.0.0.1"), 1800);
	std::cout << "connect ret: " << ret << std::endl;
	ret = tcp_sock->send(buffer);
	std::cout << "send ret: " << ret << std::endl;
}

int main()
{
	//tcp_socket_test();
	tcp_socket_test_2();
	return 0;
}
