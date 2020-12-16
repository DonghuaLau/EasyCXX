#ifndef _HELLO_HD_H_
#define _HELLO_HD_H_

#include "poller.h"

class HelloHD {
private:
    string _msg;
public:
    virtual Handler *create(FD &fd)
    {
        Handler *hd = new HelloHD();
        hd->set_fd(fd); 
        return hd;
    }
    virtual int input(char *buf, int nbuf)
    {
        cout << "[input] hello: " << buf << endl;
        msg = "I got: " + string(buf, nbuf);
    }
    virtual int output(char *buf, int max_nbuf)
    {
        cout << "[output] " << msg << endl;
        memcpy(buf, _msg.str(), _msg.size());
        return _msg_size();
    }
};

#endif

