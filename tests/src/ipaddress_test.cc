#include <bcl/ipaddress.h>
#include <gtest/gtest.h>

namespace test_ipaddress {

std::string addr4 = "17.34.51.68";

TEST(ipAddress, fromStringV4)
{
  auto ip = bcl::IPAddress(addr4);
  EXPECT_EQ(AF_INET, ip.family());	
  EXPECT_EQ(true, ip.isV4());	
  EXPECT_EQ(false, ip.isV6());	
  EXPECT_EQ(0x44332211, ip.getAddr4()->s_addr);	
  EXPECT_EQ(addr4, ip.toString());	
}

TEST(ipAddress, fromAddrV4)
{
  auto tmp = bcl::IPAddress(addr4);
  auto ip = bcl::IPAddress(tmp.getAddr4());
  EXPECT_EQ(tmp.family(), ip.family());	
  EXPECT_EQ(tmp.isV4(), ip.isV4());	
  EXPECT_EQ(tmp.isV6(), ip.isV6());	
  EXPECT_EQ(tmp.getAddr4()->s_addr, ip.getAddr4()->s_addr);	
  EXPECT_EQ(tmp.toString(), ip.toString());	
}

}