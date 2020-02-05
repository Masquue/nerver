#ifndef NERVER_TCP_CONN_H
#define NERVER_TCP_CONN_H

#include <list>

#include "socket.h"
#include "buffer.h"
#include "channel.h"
#include "tcp_server.h"

namespace nerver {

class conn_state {
    friend bool operator==(conn_state const &lhs, conn_state const &rhs);
    friend bool operator!=(conn_state const &lhs, conn_state const &rhs);

public:
    enum state {
        connected, peer_shutdown, local_shutdown, 
        waiting_death, waiting_transfer
    };

    conn_state(state s);

    operator bool() const = delete;

    std::string to_string() const;

private:
    state state_;
};

bool operator==(conn_state const &lhs, conn_state const &rhs);
bool operator!=(conn_state const &lhs, conn_state const &rhs);

class tcp_conn {
    friend class tcp_server;

public:
    using message_callback = tcp_server::message_callback;

    tcp_conn(tcp_server &server, poller &p, Socket socket, inet_addr peer);

    //  tcp_conn is noncopyable
    tcp_conn(tcp_conn const &) = delete;
    tcp_conn &operator=(tcp_conn const &) = delete;

    void set_receive_buffer_size(std::size_t sz);
    void shutdown();
    void die();

    void send(void const *data, std::size_t data_len);

    conn_state state() const;
    inet_addr peer_addr() const;

public:
    static const conn_state connected;
    static const conn_state peer_shutdown;
    static const conn_state local_shutdown;
    static const conn_state waiting_death;
    static const conn_state waiting_transfer;

private:
    using tcp_conn_iter = tcp_server::tcp_conn_iter;

    void read_handler();
    void write_handler();

    //   should be called right after construction by tcp_server
    void set_iter(tcp_conn_iter iter);
    void set_message_callback(message_callback msg_cb);

private:
    Socket socket_;
    channel channel_;
    inet_addr local_addr_, peer_addr_;
    conn_state state_;
    tcp_server &server_;
    tcp_conn_iter this_iter_;
    message_callback message_cb_;

    std::vector<char> receive_buffer_;
    buffer send_buffer_;

    static const std::size_t Default_buffer_size;
};

}   // namespace nerver {

#endif  // #ifndef NERVER_TCP_CONN_H
