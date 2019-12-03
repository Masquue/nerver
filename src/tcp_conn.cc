#include "tcp_conn.h"
#include "tcp_server.h"

namespace nerver {

tcp_conn::tcp_conn(tcp_server &server, poller &p, Socket &&socket, inet_addr peer)
    : socket_(std::move(socket)),
      channel_(p, socket_.fd()),
      local_addr_(socket_.local_addr()),
      peer_addr_(peer),
      server_(server)
{
    socket_.set_nonblock();
    channel_.set_read_callback(std::bind(&tcp_conn::read_handler, this));
    channel_.set_read();
}

void tcp_conn::die()
{
    channel_.clear_interests();
    server_.remove_connection(this_iter_);
}


inet_addr tcp_conn::peer_addr() const
{
    return peer_addr_;
}

void tcp_conn::set_iter(tcp_conn_iter iter)
{
    this_iter_ = iter;
}

void tcp_conn::set_message_callback(message_callback msg_cb)
{
    message_cb_ = msg_cb;
}

#include <unistd.h>

void tcp_conn::read_handler()
{
    const std::size_t Buf_len = 4096;
    char buf[Buf_len];
    ssize_t n = read(channel_.fd(), buf, Buf_len);
    if (message_cb_)
        message_cb_(*this, buf, n);
}

}   // namespace nerver
