#pragma once

#include <string>
#include <memory>
#include <functional>

namespace bcl {

class ServerSocket {
  public:
    ServerSocket(int af, int type, std::string ip, uint16_t port);
    virtual ~ServerSocket();
    bool isListening() const { return listening; }
  protected:
    int sock;
    bool listening;
#ifdef _WIN32
  private:
    class WSAInit {
      public:
        WSAInit();
        ~WSAInit();   
    };
#endif
};

class UdpServerSocket : public ServerSocket {
  public:
    UdpServerSocket(std::string ip, uint16_t port);
    ~UdpServerSocket();
    void ReadPacket(std::function<void(std::string,std::shared_ptr<char>)> cb, uint16_t maxPacketSize = 4096);
};

class ServerSocketUtil {
  public:
    static std::shared_ptr<ServerSocket> Create(std::string protcol, std::string ip, uint16_t port);
};

}