#pragma once

#include <string>
#include <filesystem>
#include <functional>

namespace bcl {

class FileUtil {
  public:
    static std::string FileToString(const std::filesystem::path& file);
    static void ReadByLines(const std::filesystem::path& file, std::function<void(const std::string&)> lineCb);
};

}