#include <unistd.h>

#include "tcp_conn.h"
#include "tcp_server.h"
#include "exception.h"

namespace nerver {

conn_state::conn_state(state s)
    : state_(s)
{
    // nothing
}

std::string conn_state::to_string() const
{
    switch (state_) {
    case connected:
        return "connected";
    case peer_shutdown:
        return "peer_shutdown";
    case local_shutdown:
        return "local_shutdown";
    case waiting_death:
        return "waiting_death";
    case dead:
        return "death";
    default:
        return "error";
    }
}

bool operator==(conn_state const &lhs, conn_state const &rhs)
{
    return lhs.state_ == rhs.state_;
}

bool operator!=(conn_state const &lhs, conn_state const &rhs)
{
    return !(lhs == rhs);
}

const conn_state tcp_conn::connected        = conn_state::connected;
const conn_state tcp_conn::peer_shutdown    = conn_state::peer_shutdown;
const conn_state tcp_conn::local_shutdown   = conn_state::local_shutdown;
const conn_state tcp_conn::waiting_death    = conn_state::waiting_death;
const conn_state tcp_conn::dead             = conn_state::dead;

const std::size_t tcp_conn::Default_buffer_size = 4096;

tcp_conn::tcp_conn(tcp_server &server, poller &p, Socket &&socket, inet_addr peer)
    : socket_(std::move(socket)),
      channel_(p, socket_.fd()),
      local_addr_(socket_.local_addr()),
      peer_addr_(peer),
      state_(connected),
      server_(server),
      receive_buffer_(Default_buffer_size)
{
    socket_.set_nonblock();
    channel_.set_read_callback(std::bind(&tcp_conn::read_handler, this));
    channel_.set_read();
    channel_.set_write_callback(std::bind(&tcp_conn::write_handler, this));
}

void tcp_conn::set_receive_buffer_size(std::size_t sz)
{
    receive_buffer_.resize(sz);
}

void tcp_conn::shutdown()
{
    if (state_ == peer_shutdown)
        die();
    else if (state_ == connected)
        state_ = local_shutdown;
    else
        throw std::logic_error("invalid state transition from " + state_.to_string() +
                               " to " + local_shutdown.to_string());
}

void tcp_conn::die()
{
    state_ = waiting_death;
    channel_.clear_interests();
    server_.remove_connection(this_iter_);
}

void tcp_conn::send(void const *data, std::size_t data_len)
{
    ssize_t num_written = 0;
    if (send_buffer_.empty()) { // send directly
        try {
            num_written = socket_.write(data, data_len);
        } catch (sys_error const &e) {
            if (e.error_code() == EAGAIN)
                num_written = 0;
            else if (e.error_code() == EPIPE) {
                die();
                return;
            }
            else
                throw;
        }

        // here num_written >= 0, thus cast is safe
        if (static_cast<std::size_t>(num_written) < data_len) {
            // push remaining data onto buffer,
            // start monitoring
            auto data_cp = static_cast<char const *>(data);
            send_buffer_.append(data_cp + num_written, data_len - num_written);

            channel_.set_write();
        }
    } else { // push onto buffer, sent when notified by poller
        send_buffer_.append(data, data_len);
    }
}

conn_state tcp_conn::state() const
{
    return state_;
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
    ssize_t num_received;
    try {
        num_received = socket_.read(receive_buffer_.data(), receive_buffer_.size());
    } catch (sys_error const &e) {
        if (e.error_code() == EAGAIN)
            return; // ignore spurious wake up
        throw;
    }

    if (num_received == 0) {
        if (state_ == local_shutdown)
            die();
        else if (state_ == connected)
            state_ = peer_shutdown;
        else
            throw std::logic_error("invalid state transition from " + state_.to_string() +
                                   " to " + peer_shutdown.to_string());
    }

    if (message_cb_)
        message_cb_(*this, receive_buffer_.data(), num_received);
}

void tcp_conn::write_handler()
{
    ssize_t num_written;
    try {
        num_written = socket_.write(send_buffer_.data(), send_buffer_.size());
    } catch (sys_error const &e) {
        if (e.error_code() == EAGAIN)
            return;
        else if (e.error_code() == EPIPE) {
            die();
            return;
        }
        throw;
    }

    //  here num_written >= 0
    send_buffer_.pop(num_written);
    if (send_buffer_.empty())
        channel_.set_write(false);
}

}   // namespace nerver
