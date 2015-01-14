#include <iostream>
using namespace std;

template <class T>
class Singleton {
private:
  Singleton() {}
  ~Singleton() {}
  Singleton(const Singleton&);
  const Singleton& operator=(const Singleton&);

public:
  static T& getInstance() {
    static T instance;
    return instance;
  }
};

int main() {
  int &i = Singleton<int>::getInstance();
  cout << i << endl;
  return 0;
}
