#pragma once
#include <bcl/ipaddress.h>
#ifdef _WIN32
#include "ws2tcpip.h"
#else
#include <arpa/inet.h>
#endif
#include <string>

namespace bcl {

class SocketAddress {
  public:
    SocketAddress(const std::string& addressAndPort);
    SocketAddress(const std::string& address, uint16_t port);
    SocketAddress(const IPAddress& address, uint16_t port);
    SocketAddress(const sockaddr* address);
    virtual ~SocketAddress() {}
    virtual std::string toString() const;
  protected:
    uint16_t _port;
    sockaddr _sockaddr;
  public:
    uint16_t port() const { return _port; }
    sockaddr_in* getSockaddr4() const { return (sockaddr_in*)&_sockaddr; }
    sockaddr_in6* getSockaddr6() const { return (sockaddr_in6*)&_sockaddr; }
    sockaddr* getSockaddr() const { return (sockaddr*)&_sockaddr; }
    int getSockaddrSize() const { if (isV4()) { return sizeof(sockaddr_in); } if (isV6()) { return sizeof(sockaddr_in6); } return 0; }
    sa_family_t family() const { return _sockaddr.sa_family; }
    bool isV4() const { return _sockaddr.sa_family==AF_INET; }
    bool isV6() const { return _sockaddr.sa_family==AF_INET6; }
    in_addr* getAddr4() const { return &getSockaddr4()->sin_addr; }
    in6_addr* getAddr6() const { return &getSockaddr6()->sin6_addr; }
  private:
    void initFromIPAddress(const IPAddress& address);
};

}