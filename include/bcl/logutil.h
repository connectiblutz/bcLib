#pragma once

#include <iostream>
#include <bcl/singleton.h>
#include <filesystem>
#include <memory>
#include <mutex>

namespace bcl {

class LogUtil;

class LogLine
{
  public:
    LogLine(std::shared_ptr<LogUtil> logger);
    LogLine(LogLine&& other);
    ~LogLine();
    std::shared_ptr<LogUtil> _logger;
  private:
    std::unique_lock<std::mutex> _lock;

};

class LogUtil {
  public:
    static LogLine Debug();
    ~LogUtil() { };
    std::ostream* output() { return _output; }
  protected:
    LogUtil(std::filesystem::path file);
    LogUtil(std::ostream& ostream) : selfManaged(false), _output(&ostream) {};
    friend Singleton::Strong; 
    friend LogLine;
  protected:
    bool selfManaged;
    std::ostream* _output;
    std::mutex _lineMutex;
};


template <typename T>
LogLine const & operator << (LogLine const & ll, T arg)
{
    *ll._logger->output() << arg;
    return ll;
}

}