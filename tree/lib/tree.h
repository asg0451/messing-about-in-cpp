#pragma once

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
    std::unique_ptr<std::vector<E>> ToVec();

    // pass by const ref is the way to go for the normal case i think
    static std::unique_ptr<Node> FromVec(const std::vector<E> &);
  };

} // namespace Tree

template <typename E>
std::ostream &operator<<(std::ostream &o, const Tree::Node<E> &n);

// this is dump. with templates if you just do it the obvious way you get linker
// errors you have to either put impl in header, declare all possible types of
// E, or do this
// https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function
#include "tree.cpp"
