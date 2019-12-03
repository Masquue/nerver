#include <functional>

#include "tcp_server.h"
#include "tcp_conn.h"

#include <iostream>

namespace nerver {

tcp_server::tcp_server(char const *serv, int ip_version)
    : acceptor_(loop_.get_poller(),
                inet_addr(nullptr, serv, true,
                          (ip_version == 6) ? AF_INET6 : AF_INET))
{
    using namespace std::placeholders;
    acceptor_.set_connection_callback(std::bind(&tcp_server::new_connection, this, _1, _2));
}

void tcp_server::set_establish_callback(establish_callback est_cb)
{
    establish_cb_ = est_cb;
}

void tcp_server::set_message_callback(message_callback msg_cb)
{
    message_cb_ = msg_cb;
}

void tcp_server::set_close_callback(close_callback close_cb)
{
    close_cb_ = close_cb;
}

void tcp_server::start()
{
    acceptor_.listen();
    loop_.loop();
}

std::list<tcp_conn>::size_type tcp_server::number_of_connections() const
{
    return conn_list_.size();
}

void tcp_server::new_connection(Socket socket, inet_addr peer_addr)
{
    conn_list_.emplace_front(*this, loop_.get_poller(), std::move(socket), peer_addr);
    auto &new_conn = conn_list_.front();
    new_conn.set_iter(conn_list_.begin());
    new_conn.set_message_callback(message_cb_);

    if (establish_cb_)
        establish_cb_(new_conn);
}

void tcp_server::remove_connection(tcp_conn_iter iter)
{
    if (close_cb_)
        close_cb_(*iter);

    auto erase_f = [=] { this->conn_list_.erase(iter); };
    loop_.execute(erase_f);
}

}   // namespace server {
