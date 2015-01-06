// C++11
// Multiple Thread Problems: Producer-Consumer Problem.

#include <cstdio>
#include <cstring>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

class Buffer {
private:
    int capacity_;
    int size_;
    int* buffer_;
    int front_;
    int rear_;
    std::mutex lock_;
    std::condition_variable notFull_;
    std::condition_variable notEmpty_;

public:
    Buffer(int capacity) :
        capacity_(capacity), size_(0), front_(0), rear_(0) {
        buffer_ = new int[capacity_];
    }

    ~Buffer() {
        delete[] buffer_;
    }

    void add(int id, int data) {
        std::unique_lock<std::mutex> l(lock_);
        notFull_.wait(l, [this]() { return size_ < capacity_; });
        buffer_[rear_] = data;
        rear_ = (rear_ + 1) % capacity_;
        ++size_;
        std::cout << "Produced " << id << " produced " << data << std::endl;
        notEmpty_.notify_one();
    }

    int get(int id) {
        std::unique_lock<std::mutex> l(lock_);
        notEmpty_.wait(l, [this]() { return size_ > 0; });
        int data = buffer_[front_];
        front_ = (front_ + 1) % capacity_;
        --size_;
        std::cout << "Consumer " << id << " fetched " << data << std::endl;
        notFull_.notify_one();
        return data;
    }
};

void consumer(int id, Buffer& buffer){
    for(int i = 0; i < 200; ++i){
        int value = buffer.get(id);
    }
}

void producer(int id, Buffer& buffer){
    for(int i = 0; i < 100; ++i){
        buffer.add(id, i);
    }
}
int main() {
    Buffer buffer(50);
    std::thread c1(consumer, 0, std::ref(buffer));
    //std::thread c2(consumer, 1, std::ref(buffer));
    std::thread p1(producer, 0, std::ref(buffer));
    std::thread p2(producer, 1, std::ref(buffer));

    c1.join();
    //c2.join();
    p1.join();
    p2.join();
    return 0;
}
