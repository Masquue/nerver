#ifndef NERVER_POLLER_H
#define NERVER_POLLER_H

#include <map>
#include <mutex>
#include <vector>

#include <sys/epoll.h>

#include "channel.h"

namespace nerver {

class poller {
public:
    poller();
    ~poller();

    //  poller is noncopyable
    poller(poller const &) = delete;
    poller &operator=(poller const &) = delete;

    void add(channel &c);
    void remove(channel &c);
    std::vector<epoll_event> poll(/*TODO: std::time_point timeout*/);

private:
    int epoll_fd_;
    std::mutex mutex_;
    std::map<int, epoll_event> monitored_fds_;
};

}   // namespace nerver

#endif  // #ifndef NERVER_POLLER_H
