#ifndef NERVER_SOCKET_H
#define NERVER_SOCKET_H

#include <sys/socket.h>

#include "inet_addr.h"

namespace nerver {

//  wrapper for a tcp socket (internet domain, stream socket)
class Socket {
public:
    explicit Socket(int fd);
    ~Socket();

    //  Socket is noncopyable
    Socket(Socket const &) = delete;
    Socket &operator=(Socket const &) = delete;

    Socket(Socket &&other);

    void bind(inet_addr const &addr);
    void listen(int backlog = SOMAXCONN);
    //  used for listening/passive socket only
    std::pair<Socket, inet_addr> accept();

    ssize_t read(void *buf, std::size_t buf_len);
    ssize_t write(void const *buf, std::size_t buf_len);
    void shutdown();

    int fd() const;
    inet_addr local_addr() const;

    //  optname should be among SO_KEEPALIVE, SO_REUSEADDR,
    //      SO_REUSEPORT, TCP_NODELAY
    void set_option(int optname, bool value);

    void set_nonblock();

public:
    static Socket nonblock_listening_socket(int family);

private:
    int fd_;
};

}   // namespace nerver

#endif  // #ifndef NERVER_SOCKET_H
