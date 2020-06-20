#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

namespace bcl {

template<class T>
class Channel {
public:
  void put(T item) {
    std::unique_lock<std::mutex> lk(mtx);
    q.push(item);
    cv.notify_one();
  }
  T get(){
      std::unique_lock<std::mutex> lk(mtx);
      cv.wait(lk,[this]{ return q.size()>0; });
      T ret = q.pop();
      return ret;
  }
  void close() {
    put(NULL);
  }
  friend Channel& operator<<(Channel& c, const T& item) {
    c.put(item);
    return c;
  }
  friend Channel& operator>>(Channel& c, const T& item) {
    item=c.get();
    return c;
  }
private:
  std::mutex mtx;
  std::condition_variable cv;
  std::queue<T> q;
};


}
