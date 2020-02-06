#ifndef NERVER_EVENT_LOOP_THREADED_H
#define NERVER_EVENT_LOOP_THREADED_H

#include <thread>

#include "event_loop.h"

namespace nerver {

class event_loop_threaded {
public:
    using job_t = std::function<void()>;

    event_loop_threaded(std::size_t num_of_threads);
    ~event_loop_threaded() = default;

    //  event_loop_threaded is noncopyable
    event_loop_threaded(event_loop_threaded const &) = delete;
    event_loop_threaded &operator=(event_loop_threaded const &) = delete;

    void loop();
    poller &get_poller();
    void execute(job_t job);

private:
    bool looping_;
    std::size_t next_poller_;
    std::size_t next_executor_;
    std::size_t num_of_threads_;
    std::vector<event_loop> loops_;
    std::vector<std::thread> threads_;
};

}   // namespace nerver

#endif  // #ifndef NERVER_EVENT_LOOP_THREADED_H
