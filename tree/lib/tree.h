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
    E value; // we're copying E into us which i think is right, but we take E as
             // a const ref everywhere to avoid an extra copy in those methods
    std::unique_ptr<Node> left, right;
    // NOT a unique_ptr or shared_ptr as we're just observing the value
    // Node *parent;

    Node(const E &);     // constructor (normal)
    Node(const Node &);  // constructor (copy)
    Node(const Node &&); // constructor (move)

    // like tostring. virtual == potentially overridden?
    std::ostream &Dump(std::ostream &) const;
    void Add(const E &);
    bool Contains(const E &);
    std::vector<E> ToVec(); // doesnt need to return a unique ptr or ptr bc of
                            // return value optimization

    // pass by const ref is the way to go for the normal case i think
    static Node<E> FromVec(const std::vector<E> &);
  };

} // namespace Tree

template <typename E>
std::ostream &operator<<(std::ostream &o, const Tree::Node<E> &n);

// copied from tree.cpp as i merged that in here. definitions could be written
// inline with declarations

namespace Tree {
  template <typename E> Node<E>::Node(const E &val) { value = val; }
  // another way to do the same initalization. wack syntax but ok
  // template <typename E> Node<E>::Node(E val) : value{val} {}

  // constructor (copy)
  template <typename E> Node<E>::Node(const Node &n) {
    value = n.value;
    if (n.left != nullptr) {
      left = std::make_unique<Node<E>>(*n.left);
    }
    if (n.right != nullptr) {
      right = std::make_unique<Node<E>>(*n.right);
    }
  }

  // constructor (move)
  template <typename E> Node<E>::Node(const Node &&n) {
    std::cout << "move" << std::endl;
    value = n.value;
    if (n.left != nullptr) {
      left = std::make_unique<Node<E>>(*n.left);
    }
    if (n.right != nullptr) {
      right = std::make_unique<Node<E>>(*n.right);
    }
  }

  template <typename E> void Node<E>::Add(const E &val) {
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

  template <typename E> bool Node<E>::Contains(const E &val) {
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

  // https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#r30-take-smart-pointers-as-parameters-only-to-explicitly-express-lifetime-semantics

  template <typename E> std::vector<E> Node<E>::ToVec() {
    auto vec = std::vector<E>{};
    ToVecHelper(vec);
    return vec; // return value optimization
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

  // preserved for posterity re making unique nullptr with auto
  // template <typename E>
  // std::unique_ptr<Node<E>> Node<E>::FromVec(const std::vector<E> &vec) {
  //   auto root = std::unique_ptr<Node>{nullptr};
  //   for (auto &e : vec) {
  //     if (root == nullptr) {
  //       root = std::make_unique<Node>(e);
  //     } else {
  //       root->Add(e);
  //     }
  //   }
  //   return root;
  // }

  template <typename E> Node<E> Node<E>::FromVec(const std::vector<E> &vec) {
    Node root = Node(vec[0]);
    for (auto it = begin(vec) + 1; it != end(vec); it++) {
      root.Add(*it);
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
