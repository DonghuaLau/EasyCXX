
USAGE_TARGET="make [epoll_input|epoll_client|epoll_server]"

DEBUG=-g
CFLAGS=${DEBUG}
CXXFLAGS=${DEBUG}

all:
	@echo "Usage: "${USAGE_TARGET}

epoll_input:
	gcc epoll_input.c ${CFLAGS} -o epoll_input

epoll:
	g++ epoll-server.cpp ${CXXFLAGS} -o epoll-server
	g++ epoll-client.cpp ${CXXFLAGS} -o epoll-client

kqueue:
	g++ kqueue-server.cpp ${CXXFLAGS} -o kqueue-server
	g++ kqueue-client.cpp ${CXXFLAGS} -o kqueue-client

clean:
	@echo "None"
