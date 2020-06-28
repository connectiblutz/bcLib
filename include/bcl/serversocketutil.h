#pragma once

#include <string>
#include <memory>
#include <functional>
#include <bcl/socketaddress.h>

namespace bcl {

class ServerSocket {
  public:
    ServerSocket(int type, const SocketAddress& addr);
    virtual ~ServerSocket();
    bool isListening() const { return listening; }
    void close();
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
    UdpServerSocket(const SocketAddress& addr);
    ~UdpServerSocket();
    void ReadPacket(std::function<void(const SocketAddress&,std::shared_ptr<char>,uint16_t)> cb, uint16_t maxPacketSize = 4096);
    void WritePacket(const SocketAddress& dest, char* data, uint16_t size);
};

class ServerSocketUtil {
  public:
    static std::shared_ptr<ServerSocket> Create(const std::string& protcol, const SocketAddress& addr);
};

}