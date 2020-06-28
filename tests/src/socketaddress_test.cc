#include <bcl/socketaddress.h>
#include <gtest/gtest.h>

namespace test_socketaddress {

std::string addr4 = "17.34.51.68";
uint16_t port = 443;

TEST(socketAddress, fromIpV4)
{
  auto sock = bcl::SocketAddress(bcl::IPAddress(addr4),port);
  EXPECT_EQ(AF_INET, sock.family());	
  EXPECT_EQ(true, sock.isV4());	
  EXPECT_EQ(false, sock.isV6());
  EXPECT_EQ(0x44332211, sock.getAddr4()->s_addr);	
  EXPECT_EQ(addr4+":"+std::to_string(port), sock.toString());
  EXPECT_EQ(port,sock.port());	
  EXPECT_EQ(AF_INET, sock.getSockaddr4()->sin_family);	
  EXPECT_EQ(0x44332211, sock.getSockaddr4()->sin_addr.s_addr);	
  EXPECT_EQ(htons(port), sock.getSockaddr4()->sin_port);
  EXPECT_EQ(AF_INET, sock.getSockaddr()->sa_family);
  EXPECT_EQ(sizeof(sockaddr_in),sock.getSockaddrSize());
}

TEST(socketAddress, fromStringV4)
{
  auto sock = bcl::SocketAddress(addr4,port);
  EXPECT_EQ(AF_INET, sock.family());	
  EXPECT_EQ(true, sock.isV4());	
  EXPECT_EQ(false, sock.isV6());
  EXPECT_EQ(0x44332211, sock.getAddr4()->s_addr);	
  EXPECT_EQ(addr4+":"+std::to_string(port), sock.toString());
  EXPECT_EQ(port,sock.port());	
  EXPECT_EQ(AF_INET, sock.getSockaddr4()->sin_family);	
  EXPECT_EQ(0x44332211, sock.getSockaddr4()->sin_addr.s_addr);	
  EXPECT_EQ(htons(port), sock.getSockaddr4()->sin_port);
  EXPECT_EQ(AF_INET, sock.getSockaddr()->sa_family);
  EXPECT_EQ(sizeof(sockaddr_in),sock.getSockaddrSize());
}

TEST(socketAddress, fromSockaddrV4)
{
  auto tmp = bcl::SocketAddress(addr4,port);
  auto sock = bcl::SocketAddress(tmp.getSockaddr());
  EXPECT_EQ(tmp.family(), sock.family());	
  EXPECT_EQ(tmp.isV4(), sock.isV4());	
  EXPECT_EQ(tmp.isV6(), sock.isV6());
  EXPECT_EQ(tmp.getAddr4()->s_addr, sock.getAddr4()->s_addr);	
  EXPECT_EQ(tmp.toString(), sock.toString());
  EXPECT_EQ(tmp.port(),sock.port());	
  EXPECT_EQ(tmp.getSockaddr4()->sin_family, sock.getSockaddr4()->sin_family);	
  EXPECT_EQ(tmp.getSockaddr4()->sin_addr.s_addr, sock.getSockaddr4()->sin_addr.s_addr);	
  EXPECT_EQ(tmp.getSockaddr4()->sin_port, sock.getSockaddr4()->sin_port);
  EXPECT_EQ(tmp.getSockaddr()->sa_family, sock.getSockaddr()->sa_family);
  EXPECT_EQ(tmp.getSockaddrSize(),sock.getSockaddrSize());
}

}
