#pragma once

#ifdef _WIN32
#include "ws2tcpip.h"
#else
#include <arpa/inet.h>
#endif
#include <string>

namespace bcl {

class IPAddress {
  public:
    static bool FromString(IPAddress* ipa, const std::string& addr);
    static void FromAddr(IPAddress* ipa, const in_addr* addr);
    static void FromAddr(IPAddress* ipa, const in6_addr* addr);
    IPAddress(const std::string& address);
    IPAddress(const in_addr* address);
    IPAddress(const in6_addr* address);
    virtual ~IPAddress() { }
    sa_family_t family() const { return _family; }
    bool isV4() const { return _family==AF_INET; }
    bool isV6() const { return _family==AF_INET6; }
    in_addr* getAddr4() const { return (in_addr*)_addr; }
    in6_addr* getAddr6() const { return (in6_addr*)_addr; }
    virtual std::string toString() const;
  protected:
    sa_family_t _family;
    u_char _addr[sizeof(in6_addr)];
};

}