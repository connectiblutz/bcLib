#pragma once

#include <bcl/messagethread.h>
#include <list>

namespace bcl {

class MessageThreadPool : public MessageThread {
  public:
    MessageThreadPool(uint16_t threads = 0);
    virtual ~MessageThreadPool();
    virtual void join();
    virtual void stopWhenEmpty();
  private:
    virtual bool handleMessage(std::unique_lock<std::mutex>& lk, StoredMessage& storedMessage);
    void poolLoop();
    std::queue<StoredMessage> poolQueue;
    std::list<std::thread> poolThreads;
    std::mutex poolMutex;
    std::condition_variable poolConditionVariable;
};

}