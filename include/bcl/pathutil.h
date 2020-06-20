#pragma once

#include <filesystem>


namespace bcl {

class PathUtil {
  public:
    static std::filesystem::path binaryExe();
    static std::filesystem::path binaryPath();
    static void chdir(std::filesystem::path path);
};

}