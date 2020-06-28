#include "bcl/socketaddress.h"
#include <sstream>
#include <cstring>

namespace bcl {


SocketAddress::SocketAddress(const std::string& addressAndPort)  {
  size_t colon = addressAndPort.find(':');
  _port = std::stoi(addressAndPort.substr(colon+1));
  std::string address = addressAndPort.substr(0,colon);
  initFromIPAddress(IPAddress(address));
}
SocketAddress::SocketAddress(const std::string& address, uint16_t port) : SocketAddress(IPAddress(address),port) {
}
SocketAddress::SocketAddress(const IPAddress& address, uint16_t port) : _port(port) {
  initFromIPAddress(address);
}
void SocketAddress::initFromIPAddress(const IPAddress& address) {
  memset(&_sockaddr,0,sizeof(_sockaddr)); 
  if (address.isV4()) {
    sockaddr_in* addr = (sockaddr_in*)&_sockaddr;
    addr->sin_family=address.family();
    addr->sin_port=htons(_port);
    memcpy(&addr->sin_addr,address.getAddr4(),sizeof(in_addr));
  } else if (address.isV6()) {
    sockaddr_in6* addr = (sockaddr_in6*)&_sockaddr;
    addr->sin6_family=address.family();
    addr->sin6_port=htons(_port);
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
