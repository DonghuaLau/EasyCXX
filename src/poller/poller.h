#ifndef _POLLER_H_
#define _POLLER_H_

#include <iostream>
#include <map>
using namespace std;

typedef FD int

#define MAX_SOCK_BUF_SIZE 4096

enum POLLERROR
{
    E_OK        = 0,
    E_INVALID   = -101,
    E_EXIST     = -102,
};

class Handler
{
private:
    FD _fd;
public:
    Handler():_fd(0) {}
    void set_fd(FD &fd){ _fd = fd; }
    FD get_fd(){ return _fd; }
    virtual Handler *create(FD &fd) = 0;
    virtual int input(char *buf, int nbuf) = 0;
    virtual int output(char *buf, int max_nbuf) = 0;
};

class Poller
{
public:
    Poller(){}
    virtual int add(Handler *hd, bool is_listen = false);
    virtual int del(Handler *hd) = 0;
    virtual int del(FD fd) = 0;
    virtual int poll() = 0;
};

#endif
