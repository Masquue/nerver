#include <unistd.h>

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
    LCHECK_PRINT(close(fd_));
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

std::pair<int, inet_addr> Socket::accept()
{
    sockaddr_storage ss;
    auto p_sa = reinterpret_cast<sockaddr *>(&ss);
    socklen_t addrlen = sizeof(ss);
    int conn_fd = LCHECK_THROW(::accept(fd_, p_sa, &addrlen));

    return std::make_pair(conn_fd, inet_addr(p_sa, addrlen));
}

int Socket::fd() const
{
    return fd_;
}

//  optname should be among SO_KEEPALIVE, SO_REUSEADDR,
//      SO_REUSEPORT, TCP_NODELAY
void Socket::set_option(int optname, bool value)
{
    int optval = (value ? 1 : 0);
    socklen_t optlen = sizeof(optval);
    LCHECK_PRINT(setsockopt(fd_, SOL_SOCKET, optname, &optval, optlen));
}

int Socket::nonblock_listening_socket(int family)
{
    return LCHECK_THROW(socket(family, SOCK_STREAM | SOCK_NONBLOCK, 0));
}

}   // namespace nerver
