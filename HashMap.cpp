#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>
using namespace std;

class HashSet {
private:
  struct HashNode {
    int key;
    HashNode* next;
    HashNode* prev;
    HashNode(int k = 0) : key(k), next(nullptr), prev(nullptr) {}
  };
  
  int capacity_;
  vector<vector<HashNode*>> buckets_;
  HashNode* head_;
  HashNode* tail_;

public:
  class Iterator {
  private:
    HashNode* cur_;
  public:
    Iterator(HashNode* node) : cur_(node) {}

    bool hasNext() const {
      return cur_ && cur_->next;
    }

    Iterator next() {
      Iterator ret(cur_->next);
      cur_ = cur_->next;
      return ret;
    }

    Iterator prev() {
      Iterator ret(cur_->prev);
      cur_ = cur_->prev;
      return ret;
    }

    int get() const {
      return cur_->key;
    }
  };

  HashSet(int cap) : capacity_(cap), buckets_(cap), head_(nullptr), tail_(nullptr) {}

  void insert(int key) {
    int h = key % capacity_;
    HashNode* node = new HashNode(key);
    if (!head_) {
      head_ = tail_ = node;
    } else {
      node->prev = tail_;
      tail_->next = node;
      tail_ = node;
    }
    buckets_[h].push_back(node);
  }

  int count(int key) {
    int h = key % capacity_;
    for (auto& item : buckets_[h]) {
      if (item->key == key) {
        return 1;
      }
    }
    return 0;
  }

  void remove(int key) {
    int h = key % capacity_;
    vector<HashNode*>::iterator it = buckets_[h].begin();
    vector<HashNode*>::iterator ed = buckets_[h].end();
    while (it != ed && (*it)->key != key) {
      ++it;
    }
    if (it != ed) {
      HashNode* node = *it;
      if (node->prev) {
        node->prev->next = node->next;
      }
      if (node->next) {
        node->next->prev = node->prev;
      }
      if (node == head_) {
        head_ = node->next;
      }
      if (node == tail_) {
        tail_ = node->prev;
      }
      buckets_[h].erase(it);
    }
  }

  Iterator begin() {
    return Iterator(head_);
  }
};

int main() {
  HashSet h(10);
  for (int i = 0; i < 20; ++i) {
    h.insert(i);
  }
  HashSet::Iterator it = h.begin();
  while (true) {
    cout << it.get() << endl;
    if (it.hasNext()) {
      it = it.next();
    } else {
      break;
    }
  }

  cout << "-------------------------" << endl;
  for (int i = 0; i < 15; ++i) {
    h.remove(i);
  }
  it = h.begin();
  while (true) {
    cout << it.get() << endl;
    if (it.hasNext()) {
      it = it.next();
    } else {
      break;
    }
  }

  return 0;
}
