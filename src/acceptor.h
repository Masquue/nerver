#ifndef NERVER_ACCEPTOR_H
#define NERVER_ACCEPTOR_H

#include <functional>

#include "socket.h"
#include "channel.h"

namespace nerver {

class acceptor {
public:
    using callback = std::function<void(Socket, inet_addr)>;

    acceptor(poller &p, inet_addr const &addr);

    void listen();
    void set_connection_callback(callback cb);

private:
    void read_handler();

private:
    Socket listen_socket_;
    channel listen_channel_;
    callback connection_cb_;
};

}   // namespace nerver {

#endif  // #ifndef NERVER_ACCEPTOR_H
