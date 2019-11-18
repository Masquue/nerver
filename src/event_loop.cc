#include "event_loop.h"

namespace nerver {

event_loop::event_loop()
    : looping_(false)
{
    //  nothing
}

void event_loop::loop()
{
    looping_ = true;

    while (true) {
        auto events = poller_.poll();
        for (auto const &e : events) {
            auto c = static_cast<channel *>(e.data.ptr);
            c->handle_event(e.events);
        }
    }
}

poller &event_loop::get_poller()
{
    return poller_;
}

}   // namespace nerver
