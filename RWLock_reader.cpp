#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <vector>
using namespace std;

/**
 * Read-Write Lock
 * Reader has higher priority than writer. When a reader is reading,
 * other readers can also read. Writer need to wait all the readers
 * to finish reading before writing. And only one writer can modify
 * the buffer each time.
 */ 
class RWLock {
private:
    int readerCount_;
    mutex resourceLock_; // lock for the resource
    mutex readerCountLock_; // lock for the readerCount
    mutex entryLock_;

public:
    RWLock() : readerCount_(0) {}

    void readLock() {
        lock_guard<mutex> l(readerCountLock_);
        if (readerCount_ == 0) {
            resourceLock_.lock();
        }
        ++readerCount_;
    }

    void readUnLock() {
        lock_guard<mutex> l(readerCountLock_);
        readerCount_--;
        if (readerCount_ == 0) {
            resourceLock_.unlock();
        }
    }

    void writeLock() {
        entryLock_.lock();
        resourceLock_.lock();
    }

    void writeUnLock() {
        resourceLock_.unlock();
        entryLock_.unlock();
    }
};

vector<int> buffer = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
RWLock rwLock;
mutex outputLock;

void reader(int id) {
    rwLock.readLock();
    int i = rand() % buffer.size();
    int data = buffer[i];
    {
        lock_guard<mutex> lock(outputLock);
        cout << "reader " << id << " reads buffer[" << i << "] = " << data << endl;
    }
    rwLock.readUnLock();
}

void writer(int id) {
    rwLock.writeLock();
    int i = rand() % buffer.size();
    int data = rand();
    buffer[i] = data;
    {
        lock_guard<mutex> lock(outputLock);
        cout << "writer " << id << " writes buffer[" << i << "] = " << data << endl;
    }
    rwLock.writeUnLock();
}

int main() {
    vector<thread> threads;
    // 100 writers
    for (int i = 0; i < 100; ++i) {
        threads.push_back(thread(writer, i));
    }
    // 100 readers
    for (int i = 0; i < 100; ++i) {
        threads.push_back(thread(reader, i));
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}
