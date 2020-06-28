#include "bcl/ipaddress.h"

namespace bcl {


IPAddress::IPAddress(const std::string& address) : _family(0) {
  memset(&_addr,0,sizeof(_addr)); 
  IPAddress::FromString(this,address);
}
IPAddress::IPAddress(const in_addr* address) {
  IPAddress::FromAddr(this,address);
}
IPAddress::IPAddress(const in6_addr* address) {
  IPAddress::FromAddr(this,address);
}

bool IPAddress::FromString(IPAddress* ipa, const std::string& addr) {
  if (1==inet_pton(AF_INET, addr.c_str(), &ipa->_addr)) {
    ipa->_family=AF_INET;
    return true;
  }
  if (1==inet_pton(AF_INET6, addr.c_str(), &ipa->_addr)) {
    ipa->_family=AF_INET6;
    return true;
  }
  return false;
}
void IPAddress::FromAddr(IPAddress* ipa, const in_addr* addr) {
  ipa->_family=AF_INET;
  memcpy(&ipa->_addr,addr,sizeof(in_addr));
}
void IPAddress::FromAddr(IPAddress* ipa, const in6_addr* addr) {
  ipa->_family=AF_INET6;
  memcpy(&ipa->_addr,addr,sizeof(in6_addr));
}

std::string IPAddress::toString() const {
  char str[46];
  inet_ntop(_family, &_addr, str, sizeof str);
  return str;
}

}