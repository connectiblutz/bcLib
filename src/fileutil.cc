#include "bcl/fileutil.h"

#include <fstream>
#include <sstream>

namespace bcl {


std::string FileUtil::FileToString(const std::filesystem::path& file) {
  std::ifstream t(file.string());
  if (!t.is_open()) return "";
  std::ostringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

}