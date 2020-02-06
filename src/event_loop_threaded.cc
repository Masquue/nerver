#include "event_loop_threaded.h"

namespace nerver {

event_loop_threaded::event_loop_threaded(std::size_t num_of_threads)
    : looping_(false), next_poller_(0), next_executor_(0),
      num_of_threads_(num_of_threads),
      loops_(num_of_threads)
{
    //  nothing
}

void event_loop_threaded::loop()
{
    looping_ = true;

    for (std::size_t i = 0; i < num_of_threads_; ++i)
        threads_.emplace_back(&event_loop::loop, &loops_[i]);
}

poller &event_loop_threaded::get_poller()
{
    auto &ret = loops_[next_poller_].get_poller();
    if (++next_poller_ == num_of_threads_)
        next_poller_ = 0;

    return ret;
}

void event_loop_threaded::execute(job_t job)
{
    loops_[next_executor_].execute(job);
    if (++next_executor_ == num_of_threads_)
        next_executor_ = 0;
}

}
