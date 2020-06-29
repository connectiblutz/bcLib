#pragma once

#include <string>
#include <filesystem>

namespace bcl {

class FileUtil {
  public:
    static std::string FileToString(const std::filesystem::path& file);
};

}