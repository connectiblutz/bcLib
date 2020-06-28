#include "bcl/socketutil.h"
#include "bcl/logutil.h"
#include "bcl/stringutil.h"
#ifdef _WIN32
#include "winsock2.h"
#include "ws2tcpip.h"
#else
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
#endif

namespace bcl {

#ifdef _WIN32
#define inet_pton InetPtonA
Socket::WSAInit::WSAInit() { 
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);
}
Socket::WSAInit::~WSAInit() {
  WSACleanup();
}
#endif

Socket::Socket(int type, const SocketAddress& addr) : sock(0),connected(false) {
#ifdef _WIN32
  static WSAInit wsaInit;
#endif

  LogUtil::Debug()<<"creating socket to "<<addr.toString();
  sock = socket(addr.family(), type, 0);
  if (0==connect(sock, addr.getSockaddr(), addr.getSockaddrSize())) 
  { 
    connected=true;
  }
  
}

Socket::~Socket() {
  if (sock) closesocket(sock);
}

TcpSocket::TcpSocket(const SocketAddress& addr) : Socket(SOCK_STREAM, addr) {  
}

TcpSocket::~TcpSocket() {

}

std::shared_ptr<Socket> SocketUtil::Create(std::string protocol, const SocketAddress& addr) {
  if (protocol=="tcp") return std::dynamic_pointer_cast<Socket>(std::make_shared<TcpSocket>(addr));
  return std::shared_ptr<Socket>();
}

}
