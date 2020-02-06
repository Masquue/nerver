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

    {
        std::lock_guard<std::mutex> guard(mutex_);

        if (monitored_fds_.find(c.fd()) == monitored_fds_.end())
            //  c not exist in epoll_fd_ interest list
            LCHECK_THROW(epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, c.fd(), &event));
        else
            LCHECK_THROW(epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, c.fd(), &event));
        monitored_fds_[c.fd()] = event;
    }
}

void poller::remove(channel &c)
{
    std::lock_guard<std::mutex> guard(mutex_);

    LCHECK_THROW(epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, c.fd(), nullptr));
    monitored_fds_.erase(c.fd());
}

std::vector<epoll_event> poller::poll(/*TODO: std::time_point timeout*/)
{
    static const int Epoll_wait_event_max = 10;
    
    std::vector<epoll_event> ret(Epoll_wait_event_max);
    int timeout_ms = 0;

    int num_events;
repeat_wait:
    try {
        std::lock_guard<std::mutex> guard(mutex_);
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
