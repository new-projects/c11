#include <map>
#include <iostream>
using namespace std;


template <class K, class V>
class Iterator {
private:
    typedef typename map<K, map<K, V>>::iterator OutIterator;
    typedef typename map<K, V>::iterator InIterator;
    
    OutIterator outIt_, outEnd_;
    InIterator inIt_;
    
    void skipEmpty() {
        while (outIt_ != outEnd_ && outIt_->second.empty()) {
            ++outIt_;
        }
        if (outIt_ != outEnd_) {
            inIt_ = outIt_->second.begin();
        }
     }
public:
    Iterator(map<K, map<K, V>>& mp) : outIt_(mp.begin()), outEnd_(mp.end()) {
        skipEmpty();
    } 
    
    bool hasNext() const {
        return outIt_ != outEnd_;
    }
    
    V next() {
        V value = inIt_->second;
        ++inIt_;
        if (inIt_ == outIt_->second.end()) {
            ++outIt_;
            skipEmpty();
        }
        return value;
    }
};
        
int main() {
  map<int, map<int, int>> mp;
  mp[0] = {};
  mp[1] = {{1, 100}, {2, 2}, {3, 3}};
  mp[2] = {};
  mp[4] = {{1, 9}, {2, 2}, {4, 4}};
  Iterator<int, int> it(mp);
  while (it.hasNext()) {
    cout << it.next() << endl;
  }
  return 0;
}

