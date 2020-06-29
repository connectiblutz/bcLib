#pragma once

#include <filesystem>


namespace bcl {

class PathUtil {
  public:
    static const std::filesystem::path binaryExe();
    static const std::filesystem::path binaryPath();
    static void chdir(const std::filesystem::path& path);
};

}