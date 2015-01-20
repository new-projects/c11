#include <iostream>
using namespace std;

/**
 * Reference Count Pointer.
 * 
 * Circle-reference is a special bad case for SharedPtr.
 * 
 * class A {
 *  int value;
 *  SharedPtr<B> ptr;
 * };
 *
 * class B {
 *  int value;
 *  SharedPtr<A> ptr;
 * };
 *
 * SharedPtr<A> pa = new A();
 * SharedPtr<B> pb = new B();
 * pa->ptr = pb;
 * pb->ptr = pa;
 */
template <typename T> class SharedPtr {
private:
  T *p_;
  int *count_;

public:
  SharedPtr(T *p) {
    p_ = p;
    count_ = new int(0);
    ++(*count_);
  }

  SharedPtr(const SharedPtr &other) {
    p_ = other.p_;
    count_ = other.count_;
    ++(*count_);
  }

  ~SharedPtr() {
    --(*count_);
    if (*count_ == 0) {
      if (p_) delete p_;
      delete count_;
    }
  }

  SharedPtr &operator=(const SharedPtr &other) {
    if (this == &other || p_ == other.p_) {
      return *this;
    }
    --(*count_);
    if (*count_ == 0) {
      if (p_) delete p_;
      delete count_;
    }
    p_ = other.p_;
    count_ = other.count_;
    ++(*count_);
    return *this;
  }

  T &operator*() { return *p_; }

  T *operator->() { return p_; }
};

class A {
public:
  int value_;
  A(int value) : value_(value) {}
  ~A() { cout << "destructor value = " << value_ << endl; }
};

int main() {
  SharedPtr<A> p1 = new A(1);
  SharedPtr<A> p2 = new A(2);
  SharedPtr<A> p3 = nullptr;
  p1 = p2;
  cout << p1->value_ << endl;
  return 0;
}
