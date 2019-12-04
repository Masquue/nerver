#ifndef NERVER_TCP_SERVER_H
#define NERVER_TCP_SERVER_H

#include <list>

#include "acceptor.h"
#include "event_loop.h"

namespace nerver {

class tcp_conn;

class tcp_server {
    friend class tcp_conn;

public:
    using message_callback = std::function<void(tcp_conn&, char *, std::size_t)>;
    using establish_callback = std::function<void(tcp_conn&)>;
    using close_callback = std::function<void(tcp_conn&)>;

    tcp_server(char const *serv, int ip_version);

    //  tcp_server is noncopyable
    tcp_server(tcp_server const &) = delete;
    tcp_server &operator=(tcp_server const &) = delete;

    void set_establish_callback(establish_callback est_cb);
    void set_message_callback(message_callback msg_cb);
    void set_close_callback(close_callback close_cb);
    void set_receive_buffer_size(std::size_t sz);

    void start();

    std::list<tcp_conn>::size_type number_of_connections() const;

private:
    using tcp_conn_iter = std::list<tcp_conn>::iterator;

    void remove_connection(tcp_conn_iter p);
    void new_connection(Socket socket, inet_addr peer_addr);

private:
    event_loop loop_;
    acceptor acceptor_;
    std::list<tcp_conn> conn_list_;
    establish_callback establish_cb_;
    message_callback message_cb_;
    close_callback close_cb_;
};

}   // namespace nerver {

#include "tcp_conn.h"

#endif  // #ifndef NERVER_TCP_SERVER_H
