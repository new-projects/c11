#include <string>
#include <vector>
#include <iostream>
#include <map>
using namespace std;

// return a number that appears more than n/k times in the array.
// Time Complexity: Nlog(K)
// Space Complexity: K
vector<int> findRepeats(const vector<int>& a, int k) {
  map<int, int> count;
  for (int i = 0; i < a.size(); ++i) {
    ++count[a[i]];
    if (count.size() == k) {
      for (auto& item : count) {
        if (--item.second == 0) {
          count.erase(item.first);
        }
      }
    }
  }
  vector<int> result;
  map<int, int> count2;
  for (int i = 0; i < a.size(); ++i) {
    if (count.count(a[i]) && ++count2[a[i]] >= a.size() / k + 1) {
      result.push_back(a[i]);
    }
  }
  for (auto& item : result) {
    cout << item << endl;
  }
  return result;
}

int main() {
  vector<int> a = {1, 4, 4, 4, 3, 3, 3, 5};
  findRepeats(a, 3);
  //cout << findRepeats(a, 3) << endl;
  return 0;
}

