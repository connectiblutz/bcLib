#include "bcl/messagethreadpool.h"
#include "bcl/logutil.h"

#define UNUSED(x) (void)(x)

namespace bcl {

MessageThreadPool::MessageThreadPool(uint16_t threads) {
  if (threads==0) {
    threads = std::thread::hardware_concurrency();
  }
  LogUtil::Debug()<<"using thread pool size "<<threads;
  for (auto i = 0; i < threads; i++) {
    poolThreads.push_back(std::thread(&MessageThreadPool::poolLoop,this));
  }
}

MessageThreadPool::~MessageThreadPool() {
  stop();
  join();
}


void MessageThreadPool::join() {
  static std::mutex joinMutex;
  std::lock_guard lock(joinMutex);
  for (auto&& pt : poolThreads) {
    if (pt.joinable()) pt.join();
  }
  MessageThread::join();
}

bool MessageThreadPool::handleMessage(std::unique_lock<std::mutex>& lk, StoredMessage& storedMessage) {
  UNUSED(lk);
  std::unique_lock<std::mutex> plk(poolMutex);
  if (storedMessage.message.code()==MessageThread::MSG_STOP) {
    for (unsigned i = 0; i < poolThreads.size(); i++) {
      poolQueue.push(storedMessage);
    }
    poolConditionVariable.notify_all();
    return true;
  } else {
    poolQueue.push(storedMessage);
    poolConditionVariable.notify_one();
    return false;
  }
}
void MessageThreadPool::stopWhenEmpty() {
  MessageThread::stopWhenEmpty();
  std::unique_lock<std::mutex> lk(poolMutex);
  messageQueueConditionVariable.notify_all();
}
void MessageThreadPool::poolLoop() {
  while (true) {
    std::unique_lock<std::mutex> lk(poolMutex);
    poolConditionVariable.wait(lk);
    
    // process messages until the next is a delayed
    while (!poolQueue.empty()) {
      auto storedMessage = poolQueue.front();
      poolQueue.pop();
      if (storedMessage.message.code()==MessageThread::MSG_STOP) return;
      // unlock so other workers can process messages
      lk.unlock();
      OnMessage(storedMessage.message);
      lk.lock();
    }
    if (_stopWhenEmpty && poolQueue.empty()) {
      break;
    }
  }
}

}