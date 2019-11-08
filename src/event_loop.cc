#include "event_loop.h"

namespace nerver {

event_loop::event_loop()
    : looping_(false)
{
    //  nothing
}

void event_loop::loop()
{
    while (true) {
        auto events = poller_.poll();
        for (auto const &e : events) {
            auto c = static_cast<channel *>(e.data.ptr);
            c->handle_event(e.events);
        }
    }
}

}   // namespace nerver
