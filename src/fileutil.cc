#include "bcl/fileutil.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include "bcl/logutil.h"

namespace bcl {


std::string FileUtil::FileToString(const std::filesystem::path& file) {
  auto strfile = file.string();
  std::ifstream t(strfile.c_str(), std::ifstream::in);
  if (!t.is_open()) return "";
  std::ostringstream buffer;
  buffer << t.rdbuf();
  t.close();
  return buffer.str();
}


void FileUtil::ReadByLines(const std::filesystem::path& file, std::function<void(const std::string&)> lineCb) {
  auto strfile = file.string();
  std::ifstream t(strfile.c_str(), std::ifstream::in);
  if (!t.is_open()) return;
  std::string line;
  while (std::getline(t, line))
  {
    lineCb(line);
  }
  t.close();
}

}