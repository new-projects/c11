#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
using namespace std;

template <typename T>
class ConcurrentQueue {
private:
    queue<T> queue_;
    mutex lock_;
    condition_variable notEmpty_;

public:
    ConcurrentQueue() {}

    T pop() {
        std::unique_lock<mutex> l(lock_);
        notEmpty_.wait(l, [this]() { return !queue_.empty(); });
        auto item = queue_.front();
        queue_.pop();
        return item;
    }

    void push(const T& item) {
        std::unique_lock<mutex> l(lock_);
        queue_.push(item);
        notEmpty_.notify_one();
    }
};

ConcurrentQueue<int> q;
mutex outputLock;

void producer(int id) {
    for (int i = 0; i < 200; ++i) {
        q.push(i);
        std::lock_guard<std::mutex> l(outputLock);
        cout << "Producer " << id << " push item " << i << endl;
    }
}

void consumer(int id) {
    for (int i = 0; i < 100; ++i) {
        int item = q.pop();
        std::lock_guard<std::mutex> l(outputLock);
        cout << "Consumer " << id << " pop item " << item << endl;
    }
}

int main() {
    thread p1(producer, 1);
    thread p2(producer, 2);
    thread c1(consumer, 1);
    thread c2(consumer, 2);
    thread c3(consumer, 3);
    thread c4(consumer, 4);
    p1.join();
    p2.join();
    c1.join();
    c2.join();
    c3.join();
    c4.join();
    return 0;
}
