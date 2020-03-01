#pragma once

#include <memory>
#include <vector>

namespace Tree {
class Node {
private:
  void ToVecHelper(std::vector<int> &);

public:
  int value; // no generics yet..
  std::unique_ptr<Node> left, right;

  Node(int); // constructor

  // like tostring. virtual == potentially overridden?
  std::ostream &Dump(std::ostream &) const;
  void Add(int);
  std::unique_ptr<std::vector<int>> ToVec();

  // pass by const ref is the way to go for the normal case i think
  static std::unique_ptr<Node> FromVec(const std::vector<int> &);
};
} // namespace Tree
