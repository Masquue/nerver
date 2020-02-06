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
        
        std::vector<job_t> tmp;
        {
            std::lock_guard<std::mutex> guard(job_mutex_);
            tmp.swap(jobs_);
        }

        for (auto const &job : tmp)
            job();
    }
}

poller &event_loop::get_poller()
{
    return poller_;
}

void event_loop::execute(job_t job)
{
    std::lock_guard<std::mutex> guard(job_mutex_);
    jobs_.push_back(job);
}

}   // namespace nerver
