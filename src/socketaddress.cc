#include "bcl/socketaddress.h"
#include <sstream>

namespace bcl {


SocketAddress::SocketAddress(const std::string& address, uint16_t port) : SocketAddress(IPAddress(address),port) {
}
SocketAddress::SocketAddress(const IPAddress& address, uint16_t port) : _port(port) {
  memset(&_sockaddr,0,sizeof(_sockaddr)); 
  if (address.isV4()) {
    sockaddr_in* addr = (sockaddr_in*)&_sockaddr;
    addr->sin_family=address.family();
    addr->sin_port=htons(_port);
    addr->sin_len=sizeof(sockaddr_in);
    memcpy(&addr->sin_addr,address.getAddr4(),sizeof(in_addr));
  } else if (address.isV6()) {
    sockaddr_in6* addr = (sockaddr_in6*)&_sockaddr;
    addr->sin6_family=address.family();
    addr->sin6_port=htons(_port);
    addr->sin6_len=sizeof(sockaddr_in6);
    memcpy(&addr->sin6_addr,address.getAddr6(),sizeof(in6_addr));
  }
}
SocketAddress::SocketAddress(const sockaddr* address) : _port(0) {
  if (address->sa_family==AF_INET) {
    memcpy(&_sockaddr,address,sizeof (sockaddr_in));
    _port=ntohs(((const sockaddr_in*)&_sockaddr)->sin_port);
  } else if (address->sa_family==AF_INET6) {
    memcpy(&_sockaddr,address,sizeof (sockaddr_in6));
    _port=ntohs(((const sockaddr_in6*)&_sockaddr)->sin6_port);
  }
}

std::string SocketAddress::toString() const {
  std::ostringstream str;

  if (_sockaddr.sa_family==AF_INET) {
    char tmp[46];
    inet_ntop(_sockaddr.sa_family, &((sockaddr_in*)&_sockaddr)->sin_addr, tmp, sizeof tmp);
    str << tmp;
  } else if (_sockaddr.sa_family==AF_INET6) {
    char tmp[46];
    inet_ntop(_sockaddr.sa_family, &((sockaddr_in6*)&_sockaddr)->sin6_addr, tmp, sizeof tmp);
    str << tmp;
  }
  str  << ":" << std::to_string(_port);
  return str.str();
}

}