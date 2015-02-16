#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <set>
#include <vector>
using namespace std;

class SplayTree {
private:
  struct Node {
    Node *left, *right, *parent;
    int key;
    Node(int k = 0) : left(0), right(0), parent(0), key(k) {}
  };

  // root_ node.
  Node *root_;
  // total number of tree nodes.
  size_t size_;

  void leftRotate(Node *x) {
    Node *y = x->right;
    if (y) {
      x->right = y->left;
      if (y->left) {
        y->left->parent = x;
      }
      y->parent = x->parent;
    }
    if (!x->parent) {
      root_ = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
    if (y) {
      y->left = x;
    }
    x->parent = y;
  }

  void rightRotate(Node *x) {
    Node *y = x->left;
    if (y) {
      x->left = y->right;
      if (y->right) {
        y->right->parent = x;
      }
      y->parent = x->parent;
    }
    if (!x->parent) {
      root_ = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
    if (y) {
      y->right = x;
    }
    x->parent = y;
  }

  void zig(Node *x) { rightRotate(x->parent); }

  void zag(Node *x) { leftRotate(x->parent); }

  void zigZig(Node *x) {
    rightRotate(x->parent->parent);
    rightRotate(x->parent);
  }

  void zagZag(Node *x) {
    leftRotate(x->parent->parent);
    leftRotate(x->parent);
  }

  void zigZag(Node *x) {
    leftRotate(x->parent);
    rightRotate(x->parent);
  }

  void zagZig(Node *x) {
    rightRotate(x->parent);
    leftRotate(x->parent);
  }

  void splay(Node *x) {
    while (x->parent) {
      if (!x->parent->parent) {
        if (x->parent->left == x) {
          zig(x);
        } else {
          zag(x);
        }
      } else if (x->parent->left == x && x->parent->parent->left == x->parent) {
        zigZig(x);
      } else if (x->parent->right == x &&
                 x->parent->parent->right == x->parent) {
        zagZag(x);
      } else if (x->parent->left == x &&
                 x->parent->parent->right == x->parent) {
        zagZig(x);
      } else {
        zigZag(x);
      }
    }
  }

  void inOrderTraversal(Node *x) const {
    if (!x) {
      return;
    }
    inOrderTraversal(x->left);
    cout << x->key << " ";
    inOrderTraversal(x->right);
  }

  void clear(Node *x) {
    if (x) {
      clear(x->left);
      clear(x->right);
      delete x;
    }
  }

public:
  SplayTree() : root_(0), size_(0) {}

  ~SplayTree() { clear(root_); }

  size_t size() const { return size_; }

  void insert(const int &key) {
    Node *x = root_;
    Node *p = 0;
    while (x) {
      p = x;
      if (x->key == key) {
        return;
      } else if (x->key < key) {
        x = x->right;
      } else {
        x = x->left;
      }
    }
    x = new Node(key);
    x->parent = p;
    if (!p) {
      root_ = x;
    } else if (p->key < x->key) {
      p->right = x;
    } else {
      p->left = x;
    }
    splay(x);
    ++size_;
  }

  Node *find(const int &key) const {
    Node *x = root_;
    while (x) {
      if (x->key == key) {
        return x;
      } else if (x->key < key) {
        x = x->right;
      } else {
        x = x->left;
      }
    }
    return 0;
  }

  void remove(const int &key) {
    Node *x = find(key);
    if (!x) {
      return;
    }
    splay(x);
    if (!x->left) {
      root_ = x->right;
      if (x->right)
        x->right->parent = 0;
    } else if (!x->right) {
      root_ = x->left;
      if (x->left)
        x->left->parent = 0;
    } else {
      Node *y = x->right;
      // get the minimum element of rigth subtree.
      while (y->left)
        y = y->left;
      if (y->parent != x) {
        y->parent->left = y->right;
        if (y->right)
          y->right->parent = y->parent;
        y->right = x->right;
        y->right->parent = y;
      }
      y->left = x->left;
      y->left->parent = y;
      y->parent = 0;
      root_ = y;
    }
    delete x;
    --size_;
  }

  void inOrderTraversal() const {
    inOrderTraversal(root_);
    cout << endl;
  }

  int getHeight(Node *x) {
    if (!x) return 0;
    return max(getHeight(x->left), getHeight(x->right)) + 1;
  }

  void printTree() {
    int h = getHeight(root_);
    if (h == 0) return;
    vector<string> s(h + 1, string(' ', 1 << (h + 1)));
    printTree(root_, 0, s, 1 << h);
    for (int i = 0; i <= h; ++i) {
      cout << s[i] << endl;
    }
  }

  void printTree(Node *x, int depth, vector<string>& s, int start) {
    if (!x) return;
    s[depth][start] = static_cast<char>(x->key + '0');
    int h = getHeight(x);
    if (x->left) {
      int k = 1 << (h - 1);
      for (int i = 0; i < k; ++i) {
        s[depth][start - i - 1] = '-';
      }
      printTree(x->left, depth + 1, s, start - k);
    }
    if (x->right) {
      int k = 1 << (h - 1);
      for (int i = 0; i < k; ++i) {
        s[depth][start + i + 1] = '-';
      }
      printTree(x->right, depth + 1, s, start + k);
    }
  }

};

int main() {
  SplayTree st;
  vector<int> a;
  int n = 1000000;
  set<int> marked;

/*  for (int i = 0; i < n; ++i) {
    int t = rand();
    if (marked.count(t)) {
      --i;
      continue;
    }
    marked.insert(t);
    a.push_back(t);
  }
  for (int i = 0; i < n; ++i) {
    st.insert(a[i]);
  }
  cout << "size = " << st.size() << endl;
  for (int i = 0; i < n; ++i) {
    st.remove(a[i]);
  }
  cout << "size = " << st.size() << endl;
  st.inOrderTraversal();*/
  st.insert(1);
  st.printTree();
  st.insert(2);
  st.printTree();
  st.insert(3);
  st.printTree();
  return 0;
}
