#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <bcl/priorityqueueex.h>

namespace bcl {

class MessageThreadPool;

class MessageThread {
  public:
    class Message {
      public:
        Message(uint16_t code, std::shared_ptr<void> data = nullptr) : _code(code), _data(data) { }
        uint16_t code() const { return _code; }
        template <typename T> std::shared_ptr<T> data() const { return std::static_pointer_cast<T>(_data); }
      private:
        uint16_t _code;
        std::shared_ptr<void> _data;
    };
  public:
    MessageThread();
    virtual ~MessageThread();
    void post(uint16_t code);
    void post(Message message);
    void postDelayed(Message message, std::chrono::milliseconds delay);
    void clear(uint16_t code);
    virtual void stop();
    virtual void stopWhenEmpty();
    virtual void join();
  protected:
    virtual void OnMessage(Message message) = 0;
    virtual void OnStop() { };
  private:
    class StoredMessage {
      public:
        StoredMessage(Message message) : message(message), isImmediate(true) { insertionTime = std::chrono::steady_clock::now(); }
        StoredMessage(Message message, std::chrono::steady_clock::time_point deliveryTime) : message(message), isImmediate(false), deliveryTime(deliveryTime) { insertionTime = std::chrono::steady_clock::now(); }
        Message message;
        bool isImmediate;
        std::chrono::steady_clock::time_point deliveryTime;
        std::chrono::steady_clock::time_point insertionTime;
    };
    class StoredMessageCompare {
      public:
        bool operator() (StoredMessage a, StoredMessage b)
        {
          if (a.isImmediate&&!b.isImmediate) return false;
          if (!a.isImmediate&&b.isImmediate) return true;
          if (!a.isImmediate&&!b.isImmediate) {
            if (a.deliveryTime!=b.deliveryTime) {
              return a.deliveryTime > b.deliveryTime;
            }
          }
          return a.insertionTime > b.insertionTime;
        }
    };
    class StoredMessageCodeCompare {
      public:
        bool operator() (StoredMessage a, StoredMessage b)
        {
          return a.message.code() == b.message.code();
        }
    };
  private:
    void messageLoop();
    bool _stopWhenEmpty;
    virtual bool handleMessage(std::unique_lock<std::mutex>& lk, StoredMessage& storedMessage);
    friend MessageThreadPool;
    std::thread t;
    static const uint16_t MSG_STOP;
    std::mutex messageQueueMutex;
    std::condition_variable messageQueueConditionVariable;
    priority_queue_ex<StoredMessage,std::vector<StoredMessage>,StoredMessageCompare,StoredMessageCodeCompare> messageQueue;
};

}