#include "tree.h"
#include <iostream>
#include <memory>

using namespace std;

namespace Tree {
void Node::Foo() { return; }
} // namespace Tree

int main() {
  auto n = make_unique<Tree::Node>();
  n->Foo();
  cout << "hello" << endl;
}
