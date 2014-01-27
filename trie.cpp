#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

const int MAX_S = 26;

struct TrieNode {
	TrieNode* next[MAX_S];
	bool isWord;

	TrieNode(): isWord(false) {
		fill_n(next, MAX_S, nullptr);
	}
};

class Trie {
public:
	Trie(): root(nullptr) {}

	void insert(const string& s) {
		if (root == nullptr) {
			root = new TrieNode();
		}
		TrieNode* x = root;
		for (int i = 0; i < s.length(); ++i) {
			TrieNode* &child = x->next[s[i] - 'a'];
			if (!child) {
				child = new TrieNode();
			}
			x = child;
		}
		if (!x->isWord) {
			x->isWord = true;
		}
	}

	bool find(const string& s) {
		TrieNode* x = root;
		for (int i = 0; x && i < s.length(); ++i) {
			x = x->next[s[i] - 'a'];
			if (!x) {
				return false;
			}
		}
		return x && x->isWord;
	}

	~Trie() {
		clear(root);
	}

private:
	TrieNode* root;
	
	void clear(TrieNode* root) {
		if (!root) {
			for (int i = 0; i < MAX_S; ++i) {
				clear(root->next[i]);
			}
			delete root;
		}
	}
};

int main() {
	string cmd, word;
	Trie trie;
	while (cin >> cmd) {
		if (cmd == "q") {
			break;
		}
		cin >> word;
		if (cmd == "i") {
			trie.insert(word);
			cout << "DONE" << endl;
		} else if (cmd == "f") {
			if (trie.find(word)) {
				cout << "YES" << endl;
			} else {
				cout << "NO" << endl;
			}
		}
	}
	return 0;
}

// trie2
