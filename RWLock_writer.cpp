#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <vector>
using namespace std;

/**
 * Read-Write Lock
 * Reader Priority < Writer Priority.
 */ 
class RWLock {
private:
    int readerCount_;
    int writerCount_;
    mutex resourceLock_;
    mutex readerCountLock_;
    mutex writerCountLock_;
    mutex enterLock_;

public:
    RWLock() : readerCount_(0), writerCount_(0) {}

    void readLock() {
        lock_guard<mutex> enterLock(enterLock_);
        {
            lock_guard<mutex> readerCountLock(readerCountLock_);
            if (readerCount_ == 0) {
                resourceLock_.lock();
            }
            ++readerCount_;
        }
    }

    void readUnLock() {
        lock_guard<mutex> readerCountLock(readerCountLock_);
        readerCount_--;
        if (readerCount_ == 0) {
            resourceLock_.unlock();
        }
    }

    void writeLock() {
        {
            lock_guard<mutex> writerCountLock(writerCountLock_);
            if (writerCount_ == 0) {
                enterLock_.lock();
            }
            ++writerCount_;
        }
        resourceLock_.lock();
    }

    void writeUnLock() {
        resourceLock_.unlock();
        {
            lock_guard<mutex> writerCountLock(writerCountLock_);
            --writerCount_;
            if (writerCount_ == 0) {
                enterLock_.unlock();
            }
        }
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
