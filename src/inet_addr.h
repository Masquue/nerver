#ifndef NERVER_INET_ADDR_H
#define NERVER_INET_ADDR_H

#include <utility>

#include <sys/socket.h>

namespace nerver {

class inet_addr {
public:
    inet_addr(char const *host, char const *service,
              bool passive = true, int family = AF_UNSPEC);
    inet_addr(sockaddr *addr, socklen_t addrlen);

    bool ipv6() const;
    std::pair<sockaddr *, socklen_t *> get_raw();
    std::pair<sockaddr const *, socklen_t const *> get_raw() const;
private:
    bool ipv6_;
    sockaddr_storage ss_;
    socklen_t addrlen_;
};

}   // namespace nerver

#endif  // #ifndef NERVER_INET_ADDR_H
