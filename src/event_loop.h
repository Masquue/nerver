#ifndef NERVER_EVENT_LOOP_H
#define NERVER_EVENT_LOOP_H

#include "poller.h"

namespace nerver {

class event_loop {
public:
    event_loop();
    ~event_loop() = default;

    //  event_loop is noncopyable
    event_loop(event_loop const &) = delete;
    event_loop &operator=(event_loop const &) = delete;

    void loop();
    poller &get_poller();

private:
    poller poller_;
    bool looping_;
};

}   // namespace nerver

#endif  // #ifndef NERVER_EVENT_LOOP_H
