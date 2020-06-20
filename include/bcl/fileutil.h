#pragma once

#include <string>
#include <filesystem>

namespace bcl {

class FileUtil {
  public:
    static std::string FileToString(std::filesystem::path file);
};

}