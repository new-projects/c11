#include <cstdlib>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include <chrono>

const int kMaxQueueSize = 10;
const int kMaxItemsToProduce = 100;

std::mutex mtx;
std::condition_variable queueNotFull;
std::condition_variable queueNotEmpty;
std::queue<int> q;

void produce(int item) {
    std::unique_lock<std::mutex> lock(mtx);
    while (q.size() == kMaxQueueSize) {
        // queue is full
        std::cout << "Queue is full" << std::endl;
        queueNotFull.wait(lock);
    }
    std::cout << "Produce item " << item << std::endl;
    q.push(item);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    queueNotEmpty.notify_all();
}

void consume() {
    std::unique_lock<std::mutex> lock(mtx);
    while (q.size() == 0) {
        // queue is empty
        std::cout << "Queue is empty" << std::endl;
        queueNotEmpty.wait(lock);
    }
    std::cout << "Consume item " << q.front() << std::endl;
    q.pop();
    queueNotFull.notify_all();
}

void producerTask() {
    for (int i = 0; i < kMaxItemsToProduce; ++i) {
        produce(i);
    }
}

void consumerTask() {
    for (int i = 0; i < kMaxItemsToProduce; ++i) {
        consume();
    }
}

int main() {
    std::thread consumer(consumerTask);
    std::thread producer(producerTask);
    producer.join();
    consumer.join();
    return 0;
}
