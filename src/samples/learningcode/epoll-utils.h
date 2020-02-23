#ifndef _EPOLL_UTILS_H_
#define _EPOLL_UTILS_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

using namespace std;

enum ERROR_CODE{
	RET_OK = 0,
	RET_ERR,
};

#define MAXLINE 512
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT  9876
#define INFTIM 1000

#define BUFSIZE 512

#endif
