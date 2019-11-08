#include "channel.h"

namespace nerver {

channel::channel(int fd, event_t events, callback cb)
    :   fd_(fd), interested_events_(events), event_handler_cb_(cb)
{
    //  nothing
}

int channel::fd() const
{
    return fd_;
}

channel::event_t channel::interested_events() const
{
    return interested_events_;
}

void channel::handle_event(event_t event) const
{
    event_handler_cb_(event);
}

}   // namespace nerver
