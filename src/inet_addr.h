#ifndef NERVER_INET_ADDR_H
#define NERVER_INET_ADDR_H

#include <string>
#include <utility>

#include <sys/socket.h>

namespace nerver {

class inet_addr {
public:
    inet_addr(char const *host, char const *service,
              bool passive = true, int family = AF_UNSPEC);
    inet_addr(sockaddr *addr, socklen_t addrlen);

    bool ipv6() const;
    int family() const;
    std::string to_string(bool numeric_host = true, bool numeric_serv = true) const;

//  QUESTION: why use socklen_t const * instead of socklen_t?
    std::pair<sockaddr const *, socklen_t const *> get_raw() const;

private:
    bool ipv6_;
    sockaddr_storage ss_;
    socklen_t addrlen_;
};

}   // namespace nerver

#endif  // #ifndef NERVER_INET_ADDR_H
