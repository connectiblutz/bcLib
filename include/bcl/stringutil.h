#pragma once

#include <string>
#include <vector>
#include <memory>

namespace bcl {

class StringUtil {
  public:
    static std::string reverse(const std::string& s);
    static std::string ltrim(const std::string& s);
    static std::string rtrim(const std::string& s);
    static std::string trim(const std::string& s);
    static std::string asciiToHex(const std::string& s);
    static std::vector<std::string> split(const std::string& s, const wchar_t token);
    static std::wstring toWide(const std::string& s);
    static std::string toNarrow(const std::wstring& s);
#ifdef _WIN32
    static std::shared_ptr<wchar_t> toBSTR(const std::string& s);
#endif
    static bool startsWith(const std::string& s, const std::string& needle);
    static bool endsWith(const std::string& s, const std::string& needle);
};
}