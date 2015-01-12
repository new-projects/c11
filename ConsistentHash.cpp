#include <cstdio>
#include <cstring>
#include <map>
#include <vector>
#include <unordered_map>
#include <functional>
#include <cstdlib>
#include <string>
#include <iostream>
#include <memory>
using namespace std;

template <class K, class V, class H = hash<K> > class ConsistentHash {
private:
  // key: hash value
  // value: shard id
  map<unsigned int, int> nodes_;
  unordered_map<int, string> shardNames_;
  int numShards_;
  int numVirtualNodes_;

public:
  // constructor
  ConsistentHash(int numShards, int numVirtualNodes = 4)
      : numShards_(numShards), numVirtualNodes_(numVirtualNodes) {
    hash<string> hashFunc;
    for (int i = 0; i < numShards_; ++i) {
      shardNames_[i] = "shard_" + to_string(i);
      for (int j = 0; j < numVirtualNodes_; ++j) {
        size_t h = hashFunc(shardNames_[i] + "_" + to_string(j));
        nodes_[h] = i;
      }
    }
  }

  int getShardID(const K &key) const {
    size_t h = H()(key);
    map<unsigned int, int>::const_iterator it = nodes_.lower_bound(h);
    if (it == nodes_.end()) {
      return nodes_.begin()->second;
    } else {
      return it->second;
    }
  }
};

const int kNumShards = 10;
const int kNumVirtutalNodes = 1000;
const int kNumKeys = 1000000;

int main() {
  ConsistentHash<int, int> consistentHash(kNumShards, kNumVirtutalNodes);
  vector<int> cnt(kNumShards, 0);
  for (int i = 0; i < kNumKeys; ++i) {
    cnt[consistentHash.getShardID(rand())]++;
  }
  for (int i = 0; i < kNumShards; ++i) {
    cout << cnt[i] << endl;
  }
  return 0;
}
