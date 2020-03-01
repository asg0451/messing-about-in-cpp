#include "tree.h"
#include <iostream>
#include <algorithm> // vector shuffle
// + includes from tree.h eg memory (unique_ptr), vector

using namespace std;

namespace Tree {
Node::Node(int val) { value = val; }
void Node::Add(int val) {
  if (this->value <= val) {
    if (left == nullptr) {
      left = make_unique<Tree::Node>(val);
      return;
    } else {
      return left->Add(val);
    }
  } else if (right == nullptr) {
    right = make_unique<Tree::Node>(val);
    return;
  } else {
    return right->Add(val);
  }
}

std::unique_ptr<std::vector<int>> Node::ToVec() {
  auto vec = make_unique<vector<int>>();
  // https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#r30-take-smart-pointers-as-parameters-only-to-explicitly-express-lifetime-semantics
  ToVecHelper(*vec.get()); // this is the way to do it i think
  return move(vec);
}

void Node::ToVecHelper(vector<int> &vec) {
  // ltr (rtl recursion but push back)
  if (right != nullptr) {
    right->ToVecHelper(vec);
  }
  vec.push_back(value);
  if (left != nullptr) {
    left->ToVecHelper(vec);
  }

  return;
}

unique_ptr<Node> Node::FromVec(const std::vector<int> &vec) {
  auto root = unique_ptr<Node>{nullptr};
  for (auto &e : vec) {
    if (root == nullptr) {
      root = make_unique<Node>(e);
    } else {
      root->Add(e);
    }
  }
  return root;
}

std::ostream &
operator<<(std::ostream &o, const Node &n) {
  return n.Dump(o);
}
std::ostream &Node::Dump(std::ostream &o) const {
  o << "Node(" << value;
  if (left != nullptr) {
    o << ", ";
    o << *left;
  }
  if (right != nullptr) {
    o << ", ";
    o << *right;
  }
  o << ")";
  return o;
}

// Node *Node::Left() { return this->left; }
// Node *Node::Right() { return this->right; }
// int Node::Value() { return this->value; }
} // namespace Tree

// print an int vector - for convenience. this is probably a really bad idea..
std::ostream &operator<<(std::ostream &o, const vector<int> &vec) {
  o << "Vector<int>(";
  for (auto &e : vec) {
    o << e << ", ";
  }
  o << ")";
  return o;
}

int main() {
  auto n = make_unique<Tree::Node>(1); // pass 1 to constructor
  n->Add(2);
  n->Add(0);
  cout << *n << endl;

  auto vec = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  random_shuffle(vec.begin(), vec.end());
  auto n2 = Tree::Node::FromVec(vec);
  cout << *n2 << endl;

  auto vec2 = n2->ToVec();
  cout << *vec2 << endl;
}
