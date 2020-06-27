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

ServerSocket::ServerSocket(int af, int type, std::string ip, uint16_t port) : sock(0), listening(false) {
#ifdef _WIN32
  static WSAInit wsaInit;
#endif

  LogUtil::Debug()<<"listening socket on "<<ip<<":"<<port;
  sock = socket(af, type, 0);
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = af; 
  serv_addr.sin_port = htons(port);
  inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);
  if (0==bind(sock,(struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
    listening=true;
  }
  
}

ServerSocket::~ServerSocket() {
  if (sock) closesocket(sock);
}

UdpServerSocket::UdpServerSocket(std::string ip, uint16_t port) : ServerSocket(AF_INET,SOCK_DGRAM, ip, port) {  
}

UdpServerSocket::~UdpServerSocket() {

}

void UdpServerSocket::ReadPacket(std::function<void(std::string,std::shared_ptr<char>)> cb, uint16_t maxPacketSize) {
  auto data = std::shared_ptr<char>(new char[maxPacketSize], std::default_delete<char[]>());

  struct sockaddr_in src_addr; 
  socklen_t src_len = 0;
  ssize_t read = recvfrom(sock, data.get(), maxPacketSize, 0, (struct sockaddr *)&src_addr, &src_len);
  if (read == -1) {
    listening=false;
    return;
  }
  char str[46];
  inet_ntop(src_addr.sin_family, &(src_addr.sin_addr), str, sizeof str);
  cb(str, data);
}

std::shared_ptr<ServerSocket> ServerSocketUtil::Create(std::string protocol, std::string ip, uint16_t port) {
  if (protocol=="udp") return std::dynamic_pointer_cast<ServerSocket>(std::make_shared<UdpServerSocket>(ip,port));
  return std::shared_ptr<ServerSocket>();
}

}
