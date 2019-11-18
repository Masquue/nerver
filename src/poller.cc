#include "poller.h"
#include "exception.h"

#include <unistd.h>

namespace nerver {

poller::poller() 
    : epoll_fd_(LCHECK_THROW(epoll_create1(0)))
{
    //  nothing
}

poller::~poller()
{
    LCHECK_PRINT(close(epoll_fd_));
}

void poller::add(channel &c)
{
    epoll_event event;
    event.events = c.interested_events();
    event.data.ptr = &c;
    // TODO: c.fd() already in epoll_fd_ interest list
    LCHECK_THROW(epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, c.fd(), &event));
    monitored_fds_[c.fd()] = event;
}

std::vector<epoll_event> poller::poll(/*TODO: std::time_point timeout*/)
{
    static const int Epoll_wait_event_max = 10;
    
    std::vector<epoll_event> ret(Epoll_wait_event_max);
    int timeout_ms = -1;

    int num_events;
repeat_wait:
    try {
        num_events = LCHECK_THROW(epoll_wait(epoll_fd_, ret.data(), ret.size(), timeout_ms));
    } catch (sys_error const &e) {
        if (e.error_code() == EINTR)
            goto repeat_wait;
        throw;
    }
    ret.resize(num_events);

    return ret;
}

}   // namespace nerver

