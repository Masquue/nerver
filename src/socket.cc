#include <unistd.h>
#include <fcntl.h>

#include "socket.h"
#include "exception.h"

namespace nerver {

Socket::Socket(int fd)
    : fd_(fd)
{
    // nothing
}

Socket::~Socket()
{
    if (fd_ != -1)
        LCHECK_PRINT(close(fd_));
}

Socket::Socket(Socket &&other)
    : fd_(other.fd_)
{
    other.fd_ = -1;
}

void Socket::bind(inet_addr const &addr)
{
    auto p = addr.get_raw();
    LCHECK_THROW(::bind(fd_, p.first, *p.second));
}

void Socket::listen(int backlog)
{
    LCHECK_THROW(::listen(fd_, backlog));
}

std::pair<Socket, inet_addr> Socket::accept()
{
    sockaddr_storage ss;
    auto p_sa = reinterpret_cast<sockaddr *>(&ss);
    socklen_t addrlen = sizeof(ss);
    int conn_fd = LCHECK_THROW(::accept(fd_, p_sa, &addrlen));

    return std::make_pair(Socket(conn_fd), inet_addr(p_sa, addrlen));
}

int Socket::fd() const
{
    return fd_;
}

inet_addr Socket::local_addr() const
{
    sockaddr_storage ss;
    auto p = reinterpret_cast<sockaddr *>(&ss);
    socklen_t addr_len = sizeof(ss);
    LCHECK_THROW(getsockname(fd_, p, &addr_len));

    return inet_addr(p, addr_len);
}

//  optname should be among SO_KEEPALIVE, SO_REUSEADDR,
//      SO_REUSEPORT, TCP_NODELAY
void Socket::set_option(int optname, bool value)
{
    int optval = (value ? 1 : 0);
    socklen_t optlen = sizeof(optval);
    LCHECK_PRINT(setsockopt(fd_, SOL_SOCKET, optname, &optval, optlen));
}

void Socket::set_nonblock()
{
    int flags = fcntl(fd_, F_GETFL);
    flags |= O_NONBLOCK;
    LCHECK_THROW(fcntl(fd_, F_SETFL, flags));
}

Socket Socket::nonblock_listening_socket(int family)
{
    return Socket(LCHECK_THROW(socket(family, SOCK_STREAM | SOCK_NONBLOCK, 0)));
}

}   // namespace nerver
