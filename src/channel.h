#ifndef NERVER_CHANNEL_H 
#define NERVER_CHANNEL_H 

#include <cstdint>
#include <functional>

namespace nerver {

class channel {
public:
    using callback = std::function<void(uint32_t)>;
    using event_t = std::uint32_t;

    channel(int fd, event_t events, callback cb);
    ~channel() = default;

    //  channel is noncopyable
    channel(channel const &) = delete;
    channel &operator=(channel const &) = delete;

    int fd() const;
    event_t interested_events() const;

    void handle_event(event_t events) const;

private:
    const int fd_;
    const event_t interested_events_;
    const callback event_handler_cb_;
};

}   // namespace nerver

#endif  // #ifndef NERVER_CHANNEL_H 
