#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <unordered_map>
using namespace std;

// problem description:
// randomly generate k different elements from [0, n - 1], k <= n

bool check(const vector<int>& numbers) {
  unordered_set<int> st;
  for (auto item : numbers) {
    if (st.count(item)) return false;
    st.insert(item);
  }
  return true;
}

// Time: O(n)
// Space: O(k)
vector<int> gen1(int n, int k) {
  int select = k;
  int remaining = n;
  vector<int> result;
  for (int i = 0; i < n; ++i) {
    if (rand() % remaining < select) {
      result.push_back(i);
      --select;
    }
    --remaining;
  }
  if (check(result)) cout << "yes" << endl;
  else cout << "no" << endl;
  return std::move(result);
}

// Time: O(k), when k << n
// Space: O(k)
vector<int> gen2(int n, int k) {
  unordered_set<int> s;
  vector<int> result;
  while (s.size() < k) {
    s.insert(rand() % n);
  }
  for (const auto& item : s) {
    result.push_back(item);
  }
  if (check(result)) cout << "yes" << endl;
  else cout << "no" << endl;
  return std::move(result);
}

// Time: O(k), for any k
// Space: O(k)
vector<int> gen3(int n, int k) {
  unordered_map<int, int> x;
  vector<int> result;
  for (int i = 0; i < k; ++i) {
    int r = rand() % (n - i) + i;
    if (!x.count(r)) x[r] = r;
    if (!x.count(i)) x[i] = i;
    swap(x[r], x[i]);
    result.push_back(x[i]);
  }
  if (check(result)) cout << "yes" << endl;
  else cout << "no" << endl;
  return std::move(result);
}

template <typename T>
void run(T gen, int n, int k) {
  int start = clock();
  gen(n, k);
  cout << "time used = " << (clock() - start) / 1000 << "ms" << endl;
}

int main() {
  int n, k;
  while (cin >> n >> k) {
    run(gen1, n, k);
    run(gen2, n, k);
    run(gen3, n, k);
  }
  return 0;
}
