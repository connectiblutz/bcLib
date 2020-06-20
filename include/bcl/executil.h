#pragma once

#include <string>

namespace bcl {

class ExecUtil {
  public:
    static std::string Run(std::string command);
};

}
