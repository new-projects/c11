#include <cstdio>
#include <cstring>
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
using namespace std;

const int kNumPhilosophers = 2;
mutex outputMutex;

struct ChopStick {
    ChopStick(int i) : id(i) {}
    int id;
    mutex m;
};

void eat(ChopStick* left, ChopStick* right, int philosopherID) {
    lock_guard<mutex> leftLock(left->m);
    lock_guard<mutex> rightLock(right->m);
    string pe = "Philosopher " + to_string(philosopherID) + " eats with "
    + "ChopStick " + to_string(left->id) + " and ChopStick " + to_string(right->id) + "\n";
    lock_guard<mutex> outputLock(outputMutex);
    cout << pe;
}

int main() {
    vector< unique_ptr<ChopStick> > chopsticks(kNumPhilosophers);
    for (int i = 0; i < kNumPhilosophers; ++i) {
        chopsticks[i] = std::move(unique_ptr<ChopStick>(new ChopStick(i)));
    }

    vector<thread> tasks(kNumPhilosophers);

    // pick up order for all philosophers is:
    // left_chopstick_id < right_chopstick_id
    // in order to avoid deadlock.
    tasks[0] = thread(eat,
        chopsticks[0].get(),
        chopsticks[kNumPhilosophers - 1].get(),
        0);
    for (int i = 1; i < kNumPhilosophers; ++i) {
        tasks[i] = thread(eat,
            chopsticks[i - 1].get(),
            chopsticks[i].get(),
            i);
    }
    for_each(tasks.begin(), tasks.end(), mem_fn(&thread::join));
    return 0;
}
