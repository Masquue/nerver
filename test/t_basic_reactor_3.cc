#include <cstdint>
#include <cstring>

#include <iostream>

#include <unistd.h>
#include <sys/timerfd.h>

#include "channel.h"
#include "event_loop.h"

using namespace nerver;

int timer_fd;

void timeout()
{
    static int num = 0;

    uint64_t num_expiration;
    read(timer_fd, &num_expiration, sizeof(num_expiration));
    num += num_expiration;
    std::cout << num << "th timeout\n";
}

int main()
{
    event_loop loop;

    timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    channel c(loop.get_poller(), timer_fd);
    c.set_read_callback(timeout);
    c.set_read();

    //  set timer
    itimerspec it;
    std::memset(&it, 0, sizeof(it));
    it.it_value.tv_sec = 2;
    it.it_interval.tv_sec = 3;
    timerfd_settime(timer_fd, 0, &it, nullptr);

    //  monitor
    loop.loop();
}

