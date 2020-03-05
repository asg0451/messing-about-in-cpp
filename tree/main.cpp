#include "common.h"
#include "tree.h"
#include <algorithm> // vector shuffle
#include <algorithm>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

// dont emit warning-errors for these libs
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wpedantic"
#include <benchmark/benchmark.h>
#include <sparsehash/dense_hash_map>
#include <sparsehash/sparse_hash_map>
#include <sparsehash/sparse_hash_set>
#pragma clang diagnostic pop

static std::unique_ptr<std::vector<int>> setup_large_int_vec(int size) {
  auto large_int_vec = std::make_unique<std::vector<int>>();
  for (auto i = 0; i < size; i++) {
    large_int_vec->push_back(i);
  }
  return large_int_vec;
}

static void bench_tree_from_sorted_vec(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(1000);
  for (auto _ : state) {
    auto root = Tree::Node<int>::FromVec(*large_int_vec.get());
  }
}
// Register the function as a benchmark
BENCHMARK(bench_tree_from_sorted_vec);

static void bench_tree_from_shuffled_vec(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(1000);
  random_shuffle(begin(*large_int_vec), end(*large_int_vec));
  auto root = std::make_unique<Tree::Node<int>>(-1);
  for (auto _ : state) {
    auto root = Tree::Node<int>::FromVec(*large_int_vec.get());
  }
}
BENCHMARK(bench_tree_from_shuffled_vec);

static void bench_tree_from_reversed_vec(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(1000);
  reverse(begin(*large_int_vec), end(*large_int_vec));
  auto root = std::make_unique<Tree::Node<int>>(-1);
  for (auto _ : state) {
    auto root = Tree::Node<int>::FromVec(*large_int_vec.get());
  }
}
BENCHMARK(bench_tree_from_reversed_vec);

static void bench_tree_contains(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec), end(*large_int_vec));
  auto large_int_vec_2 = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec_2), end(*large_int_vec_2));
  auto root = Tree::Node<int>::FromVec(*large_int_vec.get());
  auto cur_idx = 0;
  auto res = false;
  for (auto _ : state) {
    // [] alias for use with ->
    res = root.Contains(large_int_vec_2->at(cur_idx));
    cur_idx++;
    cur_idx %= large_int_vec_2->size();
  }
}
BENCHMARK(bench_tree_contains);

static void bench_vec_contains(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec), end(*large_int_vec));
  auto large_int_vec_2 = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec_2), end(*large_int_vec_2));
  auto cur_idx = 0;
  auto res = end(*large_int_vec);
  for (auto _ : state) {
    // [] alias for use with ->
    res = std::find(begin(*large_int_vec), end(*large_int_vec),
                    large_int_vec_2->at(cur_idx));
    cur_idx++;
    cur_idx %= large_int_vec_2->size();
  }
}
BENCHMARK(bench_vec_contains);

static void bench_umap_contains(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec), end(*large_int_vec));
  auto large_int_vec_2 = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec_2), end(*large_int_vec_2));

  // also, you can do
  // auto map2 = std::unordered_map<std::string, int>{{"x", 42}};
  auto map = std::make_unique<std::unordered_map<int, int>>();
  for (auto e : *large_int_vec) {
    map->insert({e, e}); // or std::make_pair(e, e)
  }

  auto cur_idx = 0;
  auto res = 0;
  for (auto _ : state) {
    res = map->at(large_int_vec_2->at(cur_idx));
    cur_idx++;
    cur_idx %= large_int_vec_2->size();
  }
}
BENCHMARK(bench_umap_contains);

static void bench_vec_bsearch(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec), end(*large_int_vec));
  auto large_int_vec_2 = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec_2), end(*large_int_vec_2));

  auto cur_idx = 0;
  auto res = 0;
  auto sorted = false;
  for (auto _ : state) {
    if (!sorted) {
      std::sort(begin(*large_int_vec), end(*large_int_vec)); // amortize sort
      sorted = true;
    }

    res = std::binary_search(begin(*large_int_vec), end(*large_int_vec),
                             large_int_vec_2->at(cur_idx));
    cur_idx++;
    cur_idx %= large_int_vec_2->size();
  }
}
BENCHMARK(bench_vec_bsearch);

static void bench_vec_unamortized_bsearch(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec), end(*large_int_vec));
  auto large_int_vec_2 = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec_2), end(*large_int_vec_2));

  auto cur_idx = 0;
  auto res = 0;
  for (auto _ : state) {

    // copy and sort every time
    auto sorted = *large_int_vec.get();
    std::sort(begin(sorted), end(sorted));

    res = std::binary_search(begin(*large_int_vec), end(*large_int_vec),
                             large_int_vec_2->at(cur_idx));
    cur_idx++;
    cur_idx %= large_int_vec_2->size();
  }
}
BENCHMARK(bench_vec_unamortized_bsearch);

static void bench_google_shs(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec), end(*large_int_vec));
  auto large_int_vec_2 = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec_2), end(*large_int_vec_2));

  auto set = google::sparse_hash_set<int, std::hash<int>>{};
  for (auto e : *large_int_vec) {
    set.insert(e);
  }

  auto cur_idx = 0;
  auto res = set.end();
  for (auto _ : state) {
    res = set.find(large_int_vec_2->at(cur_idx));
    cur_idx++;
    cur_idx %= large_int_vec_2->size();
  }
}
BENCHMARK(bench_google_shs);

static void bench_google_shm(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec), end(*large_int_vec));
  auto large_int_vec_2 = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec_2), end(*large_int_vec_2));

  // same interface as std::unordered_map more or less
  auto map = google::sparse_hash_map<int, int, std::hash<int>>{};
  for (auto e : *large_int_vec) {
    map.insert({e, e});
  }

  auto cur_idx = 0;
  auto res = map.end();
  for (auto _ : state) {
    res = map.find(large_int_vec_2->at(cur_idx));
    cur_idx++;
    cur_idx %= large_int_vec_2->size();
  }
}
BENCHMARK(bench_google_shm);

static void bench_google_dhm(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec), end(*large_int_vec));
  auto large_int_vec_2 = setup_large_int_vec(100000);
  random_shuffle(begin(*large_int_vec_2), end(*large_int_vec_2));

  // same interface as std::unordered_map more or less
  auto map = google::dense_hash_map<int, int, std::hash<int>>{};
  map.set_empty_key(-1); // need to call this before inserting
  // neet to set_deleted_key before deleting too but we're not so w/e
  for (auto e : *large_int_vec) {
    map.insert({e, e});
  }

  auto cur_idx = 0;
  auto res = map.end();
  for (auto _ : state) {
    res = map.find(large_int_vec_2->at(cur_idx));
    cur_idx++;
    cur_idx %= large_int_vec_2->size();
  }
}
BENCHMARK(bench_google_dhm);

// unique ptr wont fire the move constructor so these are both just objs not
// ptrs
static void bench_tree_tree_copy(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(1000);
  random_shuffle(begin(*large_int_vec), end(*large_int_vec));
  for (auto _ : state) {
    auto root = Tree::Node<int>(-1);
    auto copy = Tree::Node<int>(root);
  }
}
BENCHMARK(bench_tree_tree_copy);

// doesnt work
// static void bench_tree_tree_move(benchmark::State &state) {
//   auto large_int_vec = setup_large_int_vec(1000);
//   random_shuffle(begin(*large_int_vec), end(*large_int_vec));
//   for (auto _ : state) {
//     auto moved = Tree::Node<int>(Tree::Node<int>(-1)); // this should fire
//     move constructor but gets optimized away before then
//   }
// }
// BENCHMARK(bench_tree_tree_move);

static void bench_tree_to_vec(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(1000);
  random_shuffle(begin(*large_int_vec), end(*large_int_vec));
  auto root = Tree::Node<int>::FromVec(*large_int_vec.get());
  std::vector<int> vec;
  for (auto _ : state) {
    vec = root.ToVec();
  }
}
BENCHMARK(bench_tree_to_vec);

BENCHMARK_MAIN();

// int main() {
//   bench();
//   auto sa = string{"a"};
//   auto sb = string{"b"};
//   auto cmp = sa < sb;
//   cout << cmp << endl;

//   auto n = make_unique<Tree::Node<int>>(1); // pass 1 to constructor
//   n->Add(2);
//   n->Add(0);
//   cout << *n << endl;

//   auto vec = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//   random_shuffle(vec.begin(), vec.end());
//   auto n2 = Tree::Node<int>::FromVec(vec);
//   cout << *n2 << endl;
//   auto vec2 = n2->ToVec();
//   cout << *vec2 << endl;

//   auto vec3 = std::vector<string>{"hi", "there", "apple"};
//   // random_shuffle(vec3.begin(), vec3.end());
//   auto n3 = Tree::Node<string>::FromVec(vec3);
//   cout << *n3 << endl;
//   auto vec4 = n3->ToVec();
//   cout << *vec4 << endl;
// }
