#include "common.h"
#include "mrf.h"
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include <algorithm>  //map, filter
#include <functional> // plus
#include <numeric>    // reduce

int main() {
  auto vec = std::vector<int>{};
  for (auto i = 0; i < 10; i++) {
    vec.push_back(i);
  };

  // you can do something with execution policies to these which is like
  // auto-parallelize?

  // map
  auto vec_x2 = std::vector<int>{};
  std::transform(begin(vec), end(vec),
                 std::back_inserter(vec_x2), // what is this?
                 [](auto e) { return e * 2; });

  // https://en.cppreference.com/w/cpp/algorithm/transform
  // https://en.cppreference.com/w/cpp/iterator/back_inserter

  // filter
  auto vec_x3 = std::vector<int>{};
  std::copy_if(begin(vec), end(vec), std::back_inserter(vec_x3),
               [](auto e) { return e % 2 == 0; });

  // https://en.cppreference.com/w/cpp/algorithm/copy

  // reduce
  // std::reduce is not implemented yet on my machine? this doesnt have the
  // parallelization stuff but works otherwise
  auto res_x4 = std::accumulate(begin(vec), end(vec), 0,
                                [](auto acc, auto e) { return acc + e; });

  // equivalent to
  auto res_x5 = std::accumulate(begin(vec), end(vec), 0, std::plus<>());

  // yeah not supported yet. g++-9 on 19.10 should have it but well fuck that..
  // auto res_x6 = std::reduce(begin(vec), end(vec), 0, std::plus<>());

  std::cout << vec << std::endl;
  std::cout << vec_x2 << std::endl;
  std::cout << vec_x3 << std::endl;
  std::cout << res_x4 << std::endl;
  std::cout << res_x5 << std::endl;
}
