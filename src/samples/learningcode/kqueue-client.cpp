#include <sys/socket.h>
#include <sys/event.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

// Ref: https://github.com/yedf/kqueue-example

#define exit_if(r, ...) if(r) {printf(__VA_ARGS__); printf("[line %d] error no: %d error msg %s\n", __LINE__, errno, strerror(errno)); exit(1);}

const int kReadEvent = 1;
const int kWriteEvent = 2;
bool loop_continue = true;

void setNonBlock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    exit_if(flags<0, "fcntl failed");
    int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    exit_if(r<0, "fcntl failed");
}

void updateEvents(int efd, int fd, int events, bool modify) {
    struct kevent ev[2];
    int n = 0;
    if (events & kReadEvent) {
        EV_SET(&ev[n++], fd, EVFILT_READ, EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
    } else if (modify){
        EV_SET(&ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, (void*)(intptr_t)fd);
    }
    if (events & kWriteEvent) {
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
    } else if (modify){
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, (void*)(intptr_t)fd);
    }
    printf("%s fd %d events read %d write %d\n",
           modify ? "mod" : "add", fd, events & kReadEvent, events & kWriteEvent);
    int r = kevent(efd, ev, n, NULL, 0, NULL);
    exit_if(r, "kevent failed ");
}

void handleRead(int efd, int fd) {
    printf("%s\n", __func__);
    char buf[4096] = {0};
    int n = 0;
	int again = 0;
    while (true) {
		n=::read(fd, buf, sizeof buf);
		if(n < 0)
		{
    		if(errno == EAGAIN || errno == EWOULDBLOCK)
			{
				printf("read errno: %d, errmsg: %s, continue\n", errno, strerror(errno));
				if(again ++ > 2) break;
				continue;
			}
			else
			{
				printf("read errno: %d, errmsg: %s, break\n", errno, strerror(errno));
				break;
			}
		}
		else if(n == 0)
		{
			break;
		}
        printf("read %d bytes, content: %s\n", n, buf);
    }
    //exit_if(n<0, "read error"); //实际应用中，n<0应当检查各类错误，如EINTR
    printf("fd %d closed\n", fd);
    close(fd);
	loop_continue = false;
    //updateEvents(efd, cfd, kWriteEvent, true); 
}

void handleWrite(int efd, int fd) {
    printf("%s\n", __func__);
	char buf[4096] = {0};
	snprintf(buf, 4096, "Hi, This is Deep!");
	printf("send: %s\n", buf);
	int n = strlen(buf);
	int w = ::write(fd, buf, n);
    if(w < 0 && errno != EAGAIN)
    {
        printf("send failed, errno: %d\n", errno);
        close(fd);
        return ;
    }
    updateEvents(efd, fd, kReadEvent, true);
}

void loopEvents(int efd, int lfd, int waitms) {
    struct timespec timeout;
    timeout.tv_sec = waitms / 1000;
    timeout.tv_nsec = (waitms % 1000) * 1000 * 1000;
    const int kMaxEvents = 20;
    struct kevent activeEvs[kMaxEvents];
    int n = kevent(efd, NULL, 0, activeEvs, kMaxEvents, &timeout);
	if(n > 0) printf("kqueue wait return %d\n", n);
    for (int i = 0; i < n; i ++) {
        int fd = (int)(intptr_t)activeEvs[i].udata;
        int events = activeEvs[i].filter;
        if (events == EVFILT_READ) {
            handleRead(efd, fd);
        } else if (events == EVFILT_WRITE) {
            handleWrite(efd, fd);
        } else {
            exit_if(1, "unknown event");
        }
    }
}

int main() {

    int kqfd = kqueue();
    exit_if(kqfd < 0, "kqueue create failed");

    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    exit_if(clientfd < 0, "socket failed");

    short port = 5099;
	char host[] = "127.0.0.1";
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    //addr.sin_addr.s_addr = INADDR_ANY;
    int iRet = inet_pton(AF_INET, host, &addr.sin_addr.s_addr);
    if(0 == iRet)
    {
        printf("inet_pton failed, invalid address!\n");
        close(clientfd);
        return -1;
    }
    else if(iRet < 0)
    {
        printf("inet_pton failed\n");
        close(clientfd);
        return -1;
    }

    if(connect(clientfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        printf("connect failed\n");
        close(clientfd);
        return -1;
    }

    setNonBlock(clientfd);

    updateEvents(kqfd, clientfd, kWriteEvent, false);
    while(loop_continue) { //实际应用应当注册信号处理函数，退出时清理资源
        loopEvents(kqfd, clientfd, 10000);
    }

    return 0;
}
