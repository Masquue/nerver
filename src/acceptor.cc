#include <cassert>

#include "acceptor.h"

namespace nerver {

acceptor::acceptor(poller &p, inet_addr const &addr)
    : listen_socket_(Socket::nonblock_listening_socket(addr.family())),
      listen_channel_(p, listen_socket_.fd())
{
    listen_socket_.set_option(SO_REUSEADDR, true);
    listen_socket_.bind(addr);
    listen_channel_.set_read_callback(std::bind(&acceptor::read_handler, this));
}

void acceptor::read_handler()
{
    auto p = listen_socket_.accept();
    if (connection_cb_) {
        connection_cb_(std::move(p.first), p.second);
        // destruction of the return socket is ok
        assert(p.first.fd() == -1);
    }
}

void acceptor::listen()
{
    listen_socket_.listen();
    listen_channel_.set_read();
}
    
void acceptor::set_connection_callback(callback cb)
{
    connection_cb_ = cb;
}

}   // namespace nerver
