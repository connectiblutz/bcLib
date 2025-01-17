#pragma once

#include <bcl/messagethread.h>

#include <string>
#include <memory>
#include <bcl/singleton.h>

namespace bcl {

class WindowsService {
  public:
    void start();
    auto name() { return _name; }
    auto thread() { return _thread; }
  protected:
    WindowsService(std::string name, std::shared_ptr<MessageThread> thread);
    friend Singleton::Weak;
  private:
    std::string _name;
    std::shared_ptr<MessageThread> _thread;
};

}
