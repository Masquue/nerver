#include <sys/epoll.h>

#include "channel.h"
#include "poller.h"

namespace nerver {

const channel::event_t channel::Read_event = EPOLLIN | EPOLLPRI;
const channel::event_t channel::Write_event =  EPOLLOUT;

channel::channel(poller &p, int fd)
    :   fd_(fd), poller_(p),
        added_to_poller_(false),
        interested_events_(0)
{
    //  nothing
}

channel::~channel()
{
    remove_this_from_poller();
}

int channel::fd() const
{
    return fd_;
}

channel::event_t channel::interested_events() const
{
    return interested_events_;
}

void channel::set_read(bool value)
{
    if (value)
        interested_events_ |= Read_event;
    else
        interested_events_ &= ~Read_event;
    add_this_to_poller();
}

void channel::set_write(bool value)
{
    if (value)
        interested_events_ |= Write_event;
    else
        interested_events_ &= ~Write_event;
    add_this_to_poller();
}

void channel::set_read_callback(callback cb)
{
    read_cb_ = cb;
}

void channel::set_write_callback(callback cb)
{
    write_cb_ = cb;
}

void channel::handle_event(event_t event)
{
    if (event & Read_event) {
        if (read_cb_)
            read_cb_();
    } else if (event & Write_event) {
        if (write_cb_)
            write_cb_();
    }
}

void channel::add_this_to_poller()
{
    poller_.add(*this);
    added_to_poller_ = true;
}

void channel::remove_this_from_poller()
{
    if (added_to_poller_)
        poller_.remove(*this);
}

}   // namespace nerver
