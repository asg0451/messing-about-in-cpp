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
