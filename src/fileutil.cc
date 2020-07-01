#include "bcl/fileutil.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include "bcl/logutil.h"
#include <cstring>

namespace bcl {

#define USE_IFSTREAM 0

std::string FileUtil::FileToString(const std::filesystem::path& file) {
  auto strfile = file.string();
#if USE_IFSTREAM
  std::ifstream t(strfile.c_str(), std::ifstream::in);
  if (!t.is_open()) return "";
  std::ostringstream buffer;
  buffer << t.rdbuf();
  t.close();
  return buffer.str();
#else
  char * buffer = 0;
  long length;
  FILE * f = fopen (strfile.c_str(), "r");
  if (f)
  {
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    buffer = (char*)malloc (length);
    if (buffer)
    {
      fread (buffer, 1, length, f);
    }
    fclose (f);
  }
  if (buffer) {
    std::string ret(buffer);
    free(buffer);
    return ret;
  } else {
    return "";
  }
#endif
}


void FileUtil::ReadByLines(const std::filesystem::path& file, std::function<void(const std::string&)> lineCb) {
  auto strfile = file.string();
#if USE_IFSTREAM
  std::ifstream t(strfile.c_str(), std::ifstream::in);
  if (!t.is_open()) return;
  std::string line;
  while (std::getline(t, line))
  {
    lineCb(line);
  }
  t.close();
#else
  char buffer[1024];
  FILE *fp = fopen(strfile.c_str(), "r");
  if (fp) {
    while (fgets(buffer, sizeof(buffer), fp)) {
      buffer[strcspn(buffer, "\n")] = 0;
      lineCb(buffer);
    }
    fclose(fp);
  }
#endif
}

}