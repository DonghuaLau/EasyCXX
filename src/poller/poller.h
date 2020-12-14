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
protected:
    FD _fd;
public:
    Handler():_fd(0) {}
    FD get_fd(){ return _fd; }
    virtual Handler *accept(FD fd) = 0;
    virtual int input(char *buf, int nbuf) = 0;
    virtual int output(char *buf, int max_nbuf) = 0;
};

class Poller
{
protected:
    //map<FD, *Handler> _client_handlers;
    //map<FD, *Handler> _listen_handlers;
public:
    Poller(){}
    int add(Handler *hd, bool is_listen = false);
    /*{
        if(hd == NULL || hd->get_fd() <= 0)
        {
            return E_INVALID;
        }
        [>
         *if(_handler_map.find(hd->get_fd()) != _handler_map.end())
         *{
         *    return E_EXIST;
         *}
         *_handler_map.insert(pair<FD, *Handler>(hd->get_fd(), hd));
         *if(is_listen) _listen_fd = hd->get_fd();
         <]
        return E_OK;
    }*/

    int del(Handler *hd)
    {
        return del(hd->get_fd());
    }

    int del(FD fd);
    {
        auto it = _client_handlers.find(fd);
        if(it != _client_handlers.end())
        {
            _client_handlers.erase(it);
        }
        it = _listen_handlers.find(fd);
        if(it != _listen_handlers.end())
        {
            _listen_handlers.erase(it);
            return E_OK;
        }
        return E_OK;
    }

    virtual int poll() = 0;
};

#endif
