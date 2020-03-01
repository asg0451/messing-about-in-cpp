#include "common.h"
#include "tree.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

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
