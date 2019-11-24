#ifndef NERVER_CHANNEL_H 
#define NERVER_CHANNEL_H 

#include <cstdint>
#include <functional>

namespace nerver {

class channel {
public:
    using event_t = std::uint32_t;
    using callback = std::function<void()>;

    explicit channel(int fd);
    ~channel() = default;

    //  channel is noncopyable
    channel(channel const &) = delete;
    channel &operator=(channel const &) = delete;

    int fd() const;
    event_t interested_events() const;

    void set_read(bool value = true);
    void set_write(bool value = true);
    void set_read_callback(callback cb);
    void set_write_callback(callback cb);

    void handle_event(event_t events);

private:
    static const event_t Read_event;
    static const event_t Write_event;
private:
    const int fd_;
    event_t interested_events_;
    callback read_cb_;
    callback write_cb_;
};

}   // namespace nerver

#endif  // #ifndef NERVER_CHANNEL_H 
