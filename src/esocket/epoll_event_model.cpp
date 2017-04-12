#include "epoll_event_model.h"

namespace easycxx
{

EpollEventModel::EpollEventModel()
{
}

EpollEventModel::~EpollEventModel()
{
}

bool EpollEventModel::init(int max_events, bool is_server)
{
}

int EpollEventModel::add(const Connection &conn)
{
}

int EpollEventModel::del(const Connection &conn)
{
}

int EpollEventModel::modify(const Connection &conn)
{
}

int EpollEventModel::poll()
{
}

int EpollEventModel::select(int timeout)
{
}

int EpollEventModel::get_last_error()
{
}


