#include "tree.h"
#include <iostream>
#include <algorithm> // vector shuffle
// + includes from tree.h eg memory (unique_ptr), vector
#include <typeinfo> // typeid

// bad practice but i'm lazy
using namespace std;

namespace Tree {
  template <typename E> Node<E>::Node(E val) { value = val; }
  // another way to do the same initalization. wack syntax but ok
  // template <typename E> Node<E>::Node(E val) : value{val} {}

  template <typename E> void Node<E>::Add(E val) {
    // c++ templates use duck typing! if you add something that doesnt implement
    // <= this will error on compile
    auto less_than = val <= value; // this->value
    if (less_than && left == nullptr) {
      left = make_unique<Tree::Node<E>>(val);
    } else if (less_than) {
      left->Add(val);
    } else if (right == nullptr) {
      right = make_unique<Tree::Node<E>>(val);
    } else {
      right->Add(val);
    }
    return;
  }

  template <typename E> std::unique_ptr<std::vector<E>> Node<E>::ToVec() {
    auto vec = make_unique<vector<E>>();
    // https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#r30-take-smart-pointers-as-parameters-only-to-explicitly-express-lifetime-semantics
    ToVecHelper(*vec.get()); // this is the way to do it i think
    return move(vec);
  }

  template <typename E> void Node<E>::ToVecHelper(vector<E> &vec) {
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

template <typename E>
unique_ptr<Node<E>> Node<E>::FromVec(const std::vector<E> &vec) {
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

template <typename E>
std::ostream &operator<<(std::ostream &o, const Node<E> &n) {
  return n.Dump(o);
}

template <typename E> std::ostream &Node<E>::Dump(std::ostream &o) const {
  o << "Node(" << value << ", ";
  if (left != nullptr) {
    o << *left;
  }
  o << ", ";
  if (right != nullptr) {
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
template <typename E>
std::ostream &operator<<(std::ostream &o, const vector<E> &vec) {
  o << "Vector<" << typeid(E).name() << ">(";
  for (auto &e : vec) {
    o << e << ", ";
  }
  o << ")";
  return o;
}

int main() {
  auto sa = string{"a"};
  auto sb = string{"b"};
  auto cmp = sa < sb;
  cout << cmp << endl;

  auto n = make_unique<Tree::Node<int>>(1); // pass 1 to constructor
  n->Add(2);
  n->Add(0);
  cout << *n << endl;

  auto vec = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  random_shuffle(vec.begin(), vec.end());
  auto n2 = Tree::Node<int>::FromVec(vec);
  cout << *n2 << endl;
  auto vec2 = n2->ToVec();
  cout << *vec2 << endl;

  auto vec3 = std::vector<string>{"hi", "there", "apple"};
  // random_shuffle(vec3.begin(), vec3.end());
  auto n3 = Tree::Node<string>::FromVec(vec3);
  cout << *n3 << endl;
  auto vec4 = n3->ToVec();
  cout << *vec4 << endl;
}
