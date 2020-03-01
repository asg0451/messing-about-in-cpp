#pragma once

#include <iostream>
#include <typeinfo> // typeid
#include <vector>

// print an int vector - for convenience. this is probably a really bad idea..
template <typename E>
std::ostream &operator<<(std::ostream &o, const std::vector<E> &vec) {
  o << "Vector<" << typeid(E).name() << ">(";
  for (auto &e : vec) {
    o << e << ", ";
  }
  o << ")";
  return o;
}
