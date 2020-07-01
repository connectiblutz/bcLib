#include "bcl/stringutil.h"
#include <cctype>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <codecvt>
#ifdef _WIN32
#include <wtypes.h>
#endif

namespace bcl {

std::string StringUtil::reverse(const std::string& s) {
  return std::string(s.rbegin(),s.rend());
}

std::string StringUtil::ltrim(const std::string &s) {
  auto begin = std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c);});
  return std::string(begin,s.end());
}
std::string StringUtil::rtrim(const std::string &s) {
  auto end = std::find_if(s.rbegin(), s.rend(), [](int c) {return !std::isspace(c);});
  return StringUtil::reverse(std::string(end,s.rend()));
}
std::string StringUtil::trim(const std::string &s) {
  return StringUtil::ltrim(rtrim(s));
}

std::string StringUtil::asciiToHex(const std::string& s) {
  std::ostringstream converted;
  for (unsigned int i = 0; i < s.size(); ++i) {
    converted << std::setfill('0') << std::setw(2) << std::hex << (int)(s[i]);
  }
  return converted.str();
}

std::vector<std::string> StringUtil::split(const std::string& s, const wchar_t token) {
  std::vector<std::string> output;
  std::string::size_type prev_pos = 0, pos = 0;
  while((pos = s.find(token, pos)) != std::string::npos)
  {
    std::string substring( s.substr(prev_pos, pos-prev_pos) );
    output.push_back(substring);
    prev_pos = ++pos;
  }
  output.push_back(s.substr(prev_pos, pos-prev_pos));
  return output;
}

std::wstring StringUtil::toWide(const std::string& s) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  std::wstring wide = converter.from_bytes(s);
  return wide;
}

std::string StringUtil::toNarrow(const std::wstring& s) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  std::string narrow = converter.to_bytes(s);
  return narrow;
}

#ifdef _WIN32
std::shared_ptr<wchar_t> StringUtil::toBSTR(const std::string& s) {
  auto bstr = SysAllocString(StringUtil::toWide(s).c_str());
  auto b = std::shared_ptr<OLECHAR>(bstr,[](BSTR b){
    SysFreeString(b);
  });
  return b;
}
#endif

bool StringUtil::startsWith(const std::string& s, const std::string& needle) {
  if (s.length()<needle.length()) return false;
  return (s.compare(0,needle.length(),needle)==0);
}
bool StringUtil::endsWith(const std::string& s, const std::string& needle) {
  if (s.length()<needle.length()) return false;
  return (s.compare(s.length()-needle.length(),needle.length(),needle)==0);
}

}