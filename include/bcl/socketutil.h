#pragma once

#include <string>
#include <memory>
#include <bcl/socketaddress.h>

namespace bcl {

class Socket {
  public:
    Socket(int type, const SocketAddress& addr);
    virtual ~Socket();
    bool isConnected() const { return connected; }
  protected:
    int sock;
    bool connected;
#ifdef _WIN32
  private:
    class WSAInit {
      public:
        WSAInit();
        ~WSAInit();   
    };
#endif
};

class TcpSocket : public Socket {
  public:
    TcpSocket(const SocketAddress& addr);
    ~TcpSocket();
};

class SocketUtil {
  public:
    static std::shared_ptr<Socket> Create(std::string protcol, const SocketAddress& addr);
};

}