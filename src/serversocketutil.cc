#include "bcl/serversocketutil.h"
#include "bcl/logutil.h"
#include "bcl/stringutil.h"
#ifdef _WIN32
#include "winsock2.h"
#include "ws2tcpip.h"
#else
#include <arpa/inet.h>
#endif

namespace bcl {

#ifdef _WIN32
#define inet_pton InetPtonA
ServerSocket::WSAInit::WSAInit() { 
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);
}
ServerSocket::WSAInit::~WSAInit() {
  WSACleanup();
}
#else
#include <unistd.h>
#define closesocket close
#endif

ServerSocket::ServerSocket(int type, const SocketAddress& addr) : sock(0), listening(false) {
#ifdef _WIN32
  static WSAInit wsaInit;
#endif

  LogUtil::Debug()<<"listening socket on "<<addr.toString();
  sock = socket(addr.family(), type, 0);
  if (0!=bind(sock,addr.getSockaddr(), addr.getSockaddrSize())) {
    LogUtil::Debug()<<"failed to find to "<<addr.toString() << ", error "<<errno;
  } else {
    listening=true;
  }
  
}

ServerSocket::~ServerSocket() {
  if (sock) closesocket(sock);
}

UdpServerSocket::UdpServerSocket(const SocketAddress& addr) : ServerSocket(SOCK_DGRAM, addr) {  
}

UdpServerSocket::~UdpServerSocket() {

}

void UdpServerSocket::ReadPacket(std::function<void(const SocketAddress&,std::shared_ptr<char>,uint16_t)> cb, uint16_t maxPacketSize) {
  auto data = std::shared_ptr<char>(new char[maxPacketSize], std::default_delete<char[]>());

  struct sockaddr src_addr; 
  socklen_t src_len = sizeof src_addr;
  ssize_t read = recvfrom(sock, data.get(), maxPacketSize, 0, &src_addr, &src_len);
  if (read == -1) {
    listening=false;
    return;
  }
  auto addr = SocketAddress(&src_addr);
  cb(addr, data, read);
}
void UdpServerSocket::WritePacket(const SocketAddress& dest, char* data, uint16_t size) {
  ssize_t written = sendto(sock,data,size,0,dest.getSockaddr(),dest.getSockaddrSize());
  if (written!=size) {
    LogUtil::Debug()<<"failed to write "<<size<<" bytes, error "<<errno;
  }
}

std::shared_ptr<ServerSocket> ServerSocketUtil::Create(const std::string& protocol, const SocketAddress& addr) {
  if (protocol=="udp") return std::dynamic_pointer_cast<ServerSocket>(std::make_shared<UdpServerSocket>(addr));
  return std::shared_ptr<ServerSocket>();
}

}
