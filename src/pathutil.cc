#include "bcl/pathutil.h"
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <unistd.h>
#include <libgen.h>
#ifdef  __APPLE__
#include <sys/syslimits.h>
#else
#include <linux/limits.h>
#endif
#endif

namespace bcl {

const std::filesystem::path PathUtil::binaryExe() {
#ifdef _WIN32
  wchar_t szPath[MAX_PATH];
  if( GetModuleFileName( nullptr, szPath, MAX_PATH ) ) {
    std::filesystem::path module(szPath);
    return module;
  }
#else
  char exepath[PATH_MAX + 1] = {0};
  ssize_t count = readlink("/proc/self/exe", exepath, PATH_MAX);
  if (count != -1) {
    return std::filesystem::path(exepath);
  }
#endif
  return std::filesystem::path();
}
const std::filesystem::path PathUtil::binaryPath() {
  return PathUtil::binaryExe().parent_path();
}

void PathUtil::chdir(const std::filesystem::path& path) {
#ifdef _WIN32
  _wchdir(path.wstring().c_str());
#else
  ::chdir(path.string().c_str());
#endif
}

}