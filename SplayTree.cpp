#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <set>
#include <vector>
#include <queue>
#include <sstream>
#include <string>
#include <deque>
#include <fstream>
#include <iomanip>
using namespace std;

class SplayTree {
private:
  struct Node {
    Node *left, *right, *parent;
    int key;
    Node(int k = 0) : left(0), right(0), parent(0), key(k) {}
  };

  // the root node of the tree.
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

  // Delete the whole subtree of node x recursively.
  void clear(Node *x) {
    if (x) {
      clear(x->left);
      clear(x->right);
      delete x;
    }
  }

  // Find the maximum height of the binary tree
  int maxHeight(Node *p) const {
    if (!p)
      return 0;
    int leftHeight = maxHeight(p->left);
    int rightHeight = maxHeight(p->right);
    return (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
  }

  // Convert an integer value to string
  string intToString(int val) const {
    ostringstream ss;
    ss << val;
    return ss.str();
  }

  // Print the arm branches (eg, /    \ ) on a line
  void printBranches(int branchLen, int nodeSpaceLen, int startLen,
                     int nodesInThisLevel, const deque<Node *> &nodesQueue,
                     ostream &out) const {
    deque<Node *>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel / 2; i++) {
      out << ((i == 0) ? setw(startLen - 1) : setw(nodeSpaceLen - 2)) << ""
          << ((*iter++) ? "/" : " ");
      out << setw(2 * branchLen + 2) << "" << ((*iter++) ? "\\" : " ");
    }
    out << endl;
  }

  // Print the branches and node (eg, ___10___ )
  void printNodes(int branchLen, int nodeSpaceLen, int startLen,
                  int nodesInThisLevel, const deque<Node *> &nodesQueue,
                  ostream &out) const {
    deque<Node *>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel; i++, iter++) {
      out << ((i == 0) ? setw(startLen) : setw(nodeSpaceLen)) << ""
          << ((*iter && (*iter)->left) ? setfill('_') : setfill(' '));
      out << setw(branchLen + 2) << ((*iter) ? intToString((*iter)->key) : "");
      out << ((*iter && (*iter)->right) ? setfill('_') : setfill(' '))
          << setw(branchLen) << "" << setfill(' ');
    }
    out << endl;
  }

  // Print the leaves only (just for the bottom row)
  void printLeaves(int indentSpace, int level, int nodesInThisLevel,
                   const deque<Node *> &nodesQueue, ostream &out) const {
    deque<Node *>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel; i++, iter++) {
      out << ((i == 0) ? setw(indentSpace + 2) : setw(2 * level + 2))
          << ((*iter) ? intToString((*iter)->key) : "");
    }
    out << endl;
  }

  // Pretty formatting of a binary tree to the output stream
  void printPretty(Node *root, int level, int indentSpace, ostream &out) const {
    int h = maxHeight(root);
    int nodesInThisLevel = 1;

    int branchLen = 2 * ((1 << h) - 1) - (3 - level) * (1 << (h - 1));
    int nodeSpaceLen = 2 + (level + 1) * (1 << h);
    int startLen = branchLen + (3 - level) + indentSpace;
    deque<Node *> nodesQueue;
    nodesQueue.push_back(root);
    for (int r = 1; r < h; r++) {
      printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel,
                    nodesQueue, out);
      branchLen = branchLen / 2 - 1;
      nodeSpaceLen = nodeSpaceLen / 2 + 1;
      startLen = branchLen + (3 - level) + indentSpace;
      printNodes(branchLen, nodeSpaceLen, startLen, nodesInThisLevel,
                 nodesQueue, out);

      for (int i = 0; i < nodesInThisLevel; i++) {
        Node *currNode = nodesQueue.front();
        nodesQueue.pop_front();
        if (currNode) {
          nodesQueue.push_back(currNode->left);
          nodesQueue.push_back(currNode->right);
        } else {
          nodesQueue.push_back(NULL);
          nodesQueue.push_back(NULL);
        }
      }
      nodesInThisLevel *= 2;
    }
    printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel,
                  nodesQueue, out);
    printLeaves(indentSpace, level, nodesInThisLevel, nodesQueue, out);
    cout << endl;
  }

public:
  // Constructor
  SplayTree() : root_(0), size_(0) {}

  // Destructor
  ~SplayTree() { clear(root_); }

  // Return the total number of nodes in the tree.
  size_t size() const { return size_; }

  // Insert an element from the tree.
  //
  // @param key: the element that will be added.
  // @param print: print out the whole tree after insert is complete if print is
  // set to true.
  void insert(const int &key, bool print = false) {
    cout << "insert " << key << ":" << endl;
    Node *x = root_;
    Node *p = 0;
    while (x) {
      p = x;
      if (x->key == key) {
        if (print) {
          printTree();
        }
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
    if (print) {
      printTree();
    }
  }

  // Search an element from the tree. If not exists, NULL is returned.
  Node *find(const int &key) {
    Node *x = root_;
    Node *p = 0;
    while (x) {
      p = x;
      if (x->key == key) {
        break;
      } else if (x->key < key) {
        x = x->right;
      } else {
        x = x->left;
      }
    }
    if (x) {
      splay(x);
    } else if (p) {
      splay(p);
    }
    return x;
  }

  // Remove an element from the tree.
  //
  // @param key: the element that will be deleted.
  // @param print: print out the whole tree after remove is complete if print is
  // set to true.
  void remove(const int &key, bool print = false) {
    cout << "remove " << key << ":" << endl;
    Node *x = find(key);
    if (!x) {
      if (print) {
        printTree();
      }
      return;
    }
    // find(key) already does splay(x), so no splay(x) is needed here.
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
    if (print) {
      printTree();
    }
  }

  // Print the whole tree. The format looks like:
  //      ___6__
  //     /      \
  //   _2       7
  //  /  \
  // 1   3
  void printTree() const { printPretty(root_, 1, 0, cout); }
};

int main() {
  SplayTree st;
  vector<int> a;
  set<int> marked;
  int n = 7;

  for (int i = 0; i < n; ++i) {
    int t = rand() % n + 1;
    // remove duplicate elements.
    if (marked.count(t)) {
      --i;
      continue;
    }
    marked.insert(t);
    a.push_back(t);
  }
  for (int i = 0; i < n; ++i) {
    st.insert(a[i], true /*print*/);
  }
  for (int i = 0; i < n; ++i) {
    st.remove(a[i], true /*print*/);
  }
  return 0;
}
