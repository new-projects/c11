#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
using namespace std;

class Semaphore {
private:
  mutex mtx_;
  condition_variable cond_;
  int count_;

public:
  Semaphore(int count) : count_(count) {}
  
  void notify() {
    unique_lock<mutex> lock(mtx_);
    ++count_;
    cond_.notify_one();
  }

  void wait() {
    unique_lock<mutex> lock(mtx_);
    cond_.wait(lock, [this]() { return count_ > 0; });
    --count_;
  }
};

int main() {
  Semaphore sem(10);
  return 0;
}
