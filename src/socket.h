#ifndef NERVER_SOCKET_H
#define NERVER_SOCKET_H

#include <sys/socket.h>

#include "inet_addr.h"

namespace nerver {

//  wrapper for a tcp socket (internet domain, stream socket)
class Socket {
public:
    Socket(int fd);
    ~Socket();

    //  Socket is noncopyable
    Socket(Socket const &) = delete;
    Socket &operator=(Socket const &) = delete;

    void bind(inet_addr const &addr);
    void listen(int backlog = SOMAXCONN);
    std::pair<int, inet_addr> accept();

    int fd() const;

    //  optname should be among SO_KEEPALIVE, SO_REUSEADDR,
    //      SO_REUSEPORT, TCP_NODELAY
    void set_option(int optname, bool value);

public:
    static int nonblock_listening_socket(int family);

private:
    int fd_;
};

}   // namespace nerver

#endif  // #ifndef NERVER_SOCKET_H
