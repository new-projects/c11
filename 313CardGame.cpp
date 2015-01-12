#include <cstdio>
#include <cstring>
#include <deque>
#include <iostream>
#include <vector>
using namespace std;

int calc(int n) {
  deque<int> a;
  deque<int> b;
  for (int i = 0; i < n; ++i) {
    a.push_back(i);
  }
  
  int cnt = 0;
  while (true) {
    for (int i = 0; i < n; ++i) {
      int top = a.front();
      a.pop_front();
      if (!a.empty()) {
        int bottom = a.front();
        a.pop_front();
        a.push_back(bottom);
      }
      b.push_front(top);
    }
    ++cnt;
    bool done = true;
//    cout << "b.size() = " << b.size() << endl;
    for (int i = 0; i < n; ++i) {
      if (b[i] != i) {
        done = false;
        //break;
      }
  //    cout << b[i] << " ";
    }
//    cout << endl;
    if (done) {
      break;
    }
    a.swap(b);
  }
  return cnt;
}

int gcd(int a, int b) {
  return b == 0 ? a : gcd(b, a % b);
}

int lcm(int a, int b) {
  int gd = gcd(a, b);
//  cout << "gcd( " << a << "," << b << ") = " << gd << endl;
  return a * b / gcd(a, b);
}

int calc2(int n) {
  if (n == 1) {
    return 1;
  }
  vector<int> next(n + 1);
  deque<int> q;
  for (int i = 1; i <= n; ++i) {
    q.push_back(i);
  }
  for (int i = 1, pos = n; i <= n; ++i, --pos) {
    next[q.front()] = pos;
    q.pop_front();
    if (!q.empty()) {
      int secondTop = q.front();
      q.pop_front();
      q.push_back(secondTop);
    }
  }
  vector<bool> vst(n + 1, false);
  int res = 1;
  for (int i = 1; i <= n; ++i) {
    if (!vst[i]) {
      int cnt = 0;
      int cur = i;
      while (!vst[cur]) {
        ++cnt;
        vst[cur] = true;
        cur = next[cur];
      }
//      cout << "cnt = " << cnt << endl;
      res = lcm(res, cnt);
    }
  }
  return res;
}

int main() {
  int n;
  for (int i = 2; i <= 100; ++i) {
    //if (calc(i) != calc2(i)) {
   //   cout << "no" << endl;
   //   return 1;
   // }
    cout << i << " " << calc2(i) << endl;
  }
  cout << "yes" << endl;
  return 0;
}

