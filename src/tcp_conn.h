#ifndef NERVER_TCP_CONN_H
#define NERVER_TCP_CONN_H

#include <list>

#include "socket.h"
#include "channel.h"
#include "tcp_server.h"

namespace nerver {

class tcp_conn {
    friend class tcp_server;

public:
    using message_callback = tcp_server::message_callback;

    tcp_conn(tcp_server &server, poller &p, Socket &&socket, inet_addr peer);

    //  tcp_conn is noncopyable
    tcp_conn(tcp_conn const &) = delete;
    tcp_conn &operator=(tcp_conn const &) = delete;

    void die();

    inet_addr peer_addr() const;

private:
    using tcp_conn_iter = tcp_server::tcp_conn_iter;

    void read_handler();

    //   should be called right after construction
    void set_iter(tcp_conn_iter iter);

    void set_message_callback(message_callback msg_cb);

private:
    Socket socket_;
    channel channel_;
    inet_addr local_addr_, peer_addr_;
    tcp_server &server_;
    tcp_conn_iter this_iter_;
    message_callback message_cb_;
};

}   // namespace nerver {

#endif  // #ifndef NERVER_TCP_CONN_H
