#include <cstdint>
#include <cstring>

#include <iostream>

#include <unistd.h>
#include <sys/timerfd.h>

#include "channel.h"
#include "event_loop.h"

using namespace nerver;

int timer_fd;

void timeout(channel::event_t e)
{
    static int num = 0;

    uint64_t num_expiration;
    read(timer_fd, &num_expiration, sizeof(num_expiration));
    num += num_expiration;
    std::cout << num << "th timeout\n";
}

int main()
{
    timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    channel c(timer_fd, EPOLLIN, timeout);
    event_loop loop;
    loop.get_poller().add(c);

    //  set timer
    itimerspec it;
    std::memset(&it, 0, sizeof(it));
    it.it_value.tv_sec = 2;
    it.it_interval.tv_sec = 3;
    timerfd_settime(timer_fd, 0, &it, nullptr);

    //  monitor
    loop.loop();
}

