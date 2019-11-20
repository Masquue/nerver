#include <cstring>

#include <netdb.h>

#include "inet_addr.h"
#include "exception.h"

namespace nerver {

inet_addr::inet_addr(char const *host, char const *service,
                     bool passive, int family)
{
    addrinfo hints, *result;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_canonname = nullptr;
    hints.ai_addr = nullptr;
    hints.ai_next = nullptr;
    hints.ai_family = family;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = passive ? AI_PASSIVE : 0;

    GAICHECK_THROW(getaddrinfo(host, service, &hints, &result));

    // choose the first address in result list
    std::memcpy(&ss_, &result->ai_addr, result->ai_addrlen);
    addrlen_ = result->ai_addrlen;
    ipv6_ = (ss_.ss_family == AF_INET6);
}

inet_addr::inet_addr(sockaddr *addr, socklen_t addrlen)
    : addrlen_(addrlen)
{
    std::memcpy(&ss_, addr, addrlen);
    ipv6_ = (ss_.ss_family == AF_INET6);
}

bool inet_addr::ipv6() const
{
    return ipv6_;
}

std::pair<sockaddr *, socklen_t *> inet_addr::get_raw()
{
    return std::make_pair(reinterpret_cast<sockaddr *>(&ss_), &addrlen_);
}

std::pair<sockaddr const *, socklen_t const *> inet_addr::get_raw() const
{
    return std::make_pair(reinterpret_cast<sockaddr const *>(&ss_), &addrlen_);
}

}   // namespace nerver
