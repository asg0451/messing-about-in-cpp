#pragma once

#include "common.h"
#include <iostream>
#include <memory>
#include <vector>

namespace Tree {
  template <typename E> class Node {
  private:
    void ToVecHelper(std::vector<E> &);

  public:
    E value;
    std::unique_ptr<Node> left, right;

    Node(E); // constructor

    // like tostring. virtual == potentially overridden?
    std::ostream &Dump(std::ostream &) const;
    void Add(E);
    bool Contains(E);
    std::unique_ptr<std::vector<E>> ToVec();

    // pass by const ref is the way to go for the normal case i think
    static std::unique_ptr<Node> FromVec(const std::vector<E> &);
  };

} // namespace Tree

template <typename E>
std::ostream &operator<<(std::ostream &o, const Tree::Node<E> &n);

// copied from tree.cpp as i merged that in here. definitions could be written
// inline with declarations

namespace Tree {
  template <typename E> Node<E>::Node(E val) { value = val; }
  // another way to do the same initalization. wack syntax but ok
  // template <typename E> Node<E>::Node(E val) : value{val} {}

  template <typename E> void Node<E>::Add(E val) {
    // c++ templates use duck typing! if you add something that doesnt implement
    // <= this will error on compile
    auto less_than = val <= value; // this->value
    if (less_than && left == nullptr) {
      left = std::make_unique<Tree::Node<E>>(val);
    } else if (less_than) {
      left->Add(val);
    } else if (right == nullptr) {
      right = std::make_unique<Tree::Node<E>>(val);
    } else {
      right->Add(val);
    }
    return;
  }

  template <typename E> bool Node<E>::Contains(E val) {
    if (value == val) {
      return true;
    }
    auto less_than = val <= value; // this->value
    if (less_than && left == nullptr) {
      return false;
    } else if (less_than) {
      return left->Contains(val);
    } else if (right == nullptr) {
      return false;
    } else {
      return right->Contains(val);
    }
  }

  template <typename E> std::unique_ptr<std::vector<E>> Node<E>::ToVec() {
    auto vec = std::make_unique<std::vector<E>>();
    // https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#r30-take-smart-pointers-as-parameters-only-to-explicitly-express-lifetime-semantics
    ToVecHelper(*vec.get()); // this is the way to do it i think
    return move(vec);
  }

  template <typename E> void Node<E>::ToVecHelper(std::vector<E> &vec) {
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
  std::unique_ptr<Node<E>> Node<E>::FromVec(const std::vector<E> &vec) {
    auto root = std::unique_ptr<Node>{nullptr};
    for (auto &e : vec) {
      if (root == nullptr) {
        root = std::make_unique<Node>(e);
      } else {
        root->Add(e);
      }
    }
    return root;
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
} // namespace Tree

template <typename E>
std::ostream &operator<<(std::ostream &o, const Tree::Node<E> &n) {
  return n.Dump(o);
}
