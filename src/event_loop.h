#ifndef NERVER_EVENT_LOOP_H
#define NERVER_EVENT_LOOP_H

#include <mutex> 
#include "poller.h"

namespace nerver {

class event_loop {
public:
    using job_t = std::function<void()>;

    event_loop();
    ~event_loop() = default;

    //  event_loop is noncopyable
    event_loop(event_loop const &) = delete;
    event_loop &operator=(event_loop const &) = delete;

    void loop();
    poller &get_poller();
    void execute(job_t job);

private:
    poller poller_;
    bool looping_;
    std::mutex job_mutex_;
    std::vector<job_t> jobs_;
};

}   // namespace nerver

#endif  // #ifndef NERVER_EVENT_LOOP_H
