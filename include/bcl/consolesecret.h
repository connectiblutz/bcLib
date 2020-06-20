#pragma once

#include <string>

namespace bcl {

class ConsoleSecret {
    public:
        static std::string Get(const std::string& prompt);
};

}