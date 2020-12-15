#include "epoll-utils.h"

void setnonblock(int fd)
{
    int flag = fcntl ( fd, F_GETFL, 0 );
    fcntl ( fd, F_SETFL, flag | O_NONBLOCK );
}

void usage(char *name)
{
	printf("Usage: %s <host> <port> <message>", name);
}

int main(int argc, char** argv)
{
    int iRet = RET_OK;

    if(4 != argc)
    {
        //printf("Parameter: ServerIP Message ServerPort\n");
        usage(argv[0]);
        return RET_ERR;
    }

	const char *host = argv[1];
    in_port_t port = atoi(argv[2]);
    if(0 >= port)
    {
        printf("port number is wrong\n");
        return RET_ERR;
    }
	const char *msg = argv[3];

    int sk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(-1 == sk)
    {
        printf("open socket failed!\n");
        return RET_ERR;
    }


    struct sockaddr_in sa = {0};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);

    struct sockaddr_in *psa = &sa;

    iRet = inet_pton(AF_INET, host, &psa->sin_addr.s_addr);
    if(0 == iRet)
    {
        printf("inet_pton failed, invalid address!\n");
        close(sk);
        return RET_ERR;
    }
    else if(iRet < 0)
    {
        printf("inet_pton failed\n");
        close(sk);
        return RET_ERR;
    }

    if(connect(sk, (struct sockaddr*)&sa, sizeof(sa)) < 0)
    {
        printf("connect failed\n");
        close(sk);
        return RET_ERR;
    }

    setnonblock(sk);

    int efd;
    efd = epoll_create(10);
    if(efd == -1)
    {
        perror("epoll_create error!");
        exit(1);
    }

    struct epoll_event event;
    struct epoll_event events[10];

    event.events = EPOLLOUT | EPOLLIN | EPOLLET;
    event.data.fd = sk;

    epoll_ctl(efd, EPOLL_CTL_ADD, sk, &event);


    getchar();
    int loop = 0;
    while(1)
    {
        ssize_t numBytesRcvd = 0;
        char buffer[BUFSIZE] = {0};
        int n = 0;
        int i = 0;

        if(loop == 1)
        {
            break;
        }

        n = epoll_wait(efd, events, 10, -1);

        printf("%d\n", n);

        for(i = 0; i < n; i++)
        {
            if(events[i].events & EPOLLOUT)
            {
                printf("EPOLLOUT\n");
                snprintf(buffer, BUFSIZE, "i am process %d, just say: %s.", getpid(), msg);

                int n = strlen(buffer);
                int nsend = 0;

                while(n > 0)
                {
                    //nsend = send(events[i].data.fd, buffer + nsend, n, 0);
                    nsend = write(events[i].data.fd, buffer + nsend, n);
                    if(nsend < 0 && errno != EAGAIN)
                    {

                        printf("send failed\n");
                        close(events[i].data.fd);
                        return RET_ERR;
                    }
                    n -= nsend;
                }
            }

            if(events[i].events & EPOLLIN)
            {
                printf("EPOLLIN\n");
                memset(buffer, 0, BUFSIZE);

                int len = strlen(buffer);
                int n = 0;
                int nrecv = 0;

                //while((nrecv = recv(events[i].data.fd, buffer + n, BUFSIZE - 1, 0)) > 0)
                while(1){
                    nrecv = read(events[i].data.fd, buffer + n, BUFSIZE - 1) ;
                    if(nrecv == -1 && errno != EAGAIN)
                    {
                        perror("read error!");
                    }
                    if((nrecv == -1 && errno == EAGAIN) || nrecv == 0) 
                    {
                        break;
                    }
                    n += nrecv;
                }
                loop = 1;
                printf("%s\n", buffer);
            }
        }
    }
    close(sk);
    close(efd);
    return RET_OK;
}
