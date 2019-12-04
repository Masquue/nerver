#include <unistd.h>

#include "tcp_conn.h"
#include "tcp_server.h"

namespace nerver {

const std::size_t tcp_conn::Default_buffer_size = 4096;

tcp_conn::tcp_conn(tcp_server &server, poller &p, Socket &&socket, inet_addr peer)
    : socket_(std::move(socket)),
      channel_(p, socket_.fd()),
      local_addr_(socket_.local_addr()),
      peer_addr_(peer),
      server_(server),
      receive_buffer_(Default_buffer_size)
{
    socket_.set_nonblock();
    channel_.set_read_callback(std::bind(&tcp_conn::read_handler, this));
    channel_.set_read();
}

void tcp_conn::set_receive_buffer_size(std::size_t sz)
{
    receive_buffer_.resize(sz);
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

void tcp_conn::read_handler()
{
    ssize_t n = socket_.read(receive_buffer_.data(), n);
    if (message_cb_)
        message_cb_(*this, receive_buffer_.data(), n);
}

}   // namespace nerver
