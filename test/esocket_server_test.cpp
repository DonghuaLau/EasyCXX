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
	easycxx::TCPSocket tcp_sock("127.0.0.1", 1800);
	tcp_sock.be_server();

	//tcp_sock.connect(std::string("127.0.0.1"), 1800);

	int conn = 0;
	
	easycxx::EByteBuffer buffer(4096);

	while(true){
		conn = tcp_sock.accept();
		if(conn <= 0){
			printf("accept error, conn: %d, errno: %d\n", conn, errno);
			break;
		}
	
		// read data
		tcp_sock.recv(conn, buffer);
		printf("conn: %d, recv: %s\n", conn, buffer.get_buffer());
		tcp_sock.close(conn);
		buffer.clear();
	}
}

int main()
{
	tcp_socket_test();
	return 0;
}
