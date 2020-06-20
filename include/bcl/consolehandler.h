#pragma once

#include <bcl/messagethread.h>
#include <bcl/singleton.h>

namespace bcl {

class ConsoleHandler {
  public:
    auto thread() { return _thread; }
  protected:
    ConsoleHandler(std::shared_ptr<MessageThread> thread);
    ~ConsoleHandler();
    friend Singleton::Weak;
  private:
    std::shared_ptr<MessageThread> _thread;
};

}
