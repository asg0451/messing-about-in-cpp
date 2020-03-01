#include "common.h"
#include "tree.h"
#include <algorithm> // vector shuffle
#include <benchmark/benchmark.h>
#include <iostream>
#include <memory>
#include <sparsehash/sparse_hash_map>
#include <sparsehash/sparse_hash_set>
#include <unordered_map>
#include <vector>

static std::unique_ptr<std::vector<int>> setup_large_int_vec(int size) {
  auto large_int_vec = std::make_unique<std::vector<int>>();
  for (auto i = 0; i < 1000; i++) {
    large_int_vec->push_back(i);
  }
  return std::move(large_int_vec);
}

static void bench_tree_from_sorted_vec(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(1000);
  auto root = std::make_unique<Tree::Node<int>>(-1);
  for (auto _ : state) {
    root = Tree::Node<int>::FromVec(*large_int_vec.get());
  }
}
// Register the function as a benchmark
BENCHMARK(bench_tree_from_sorted_vec);

static void bench_tree_from_shuffled_vec(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(1000);
  random_shuffle(large_int_vec->begin(), large_int_vec->end());
  auto root = std::make_unique<Tree::Node<int>>(-1);
  for (auto _ : state) {
    root = Tree::Node<int>::FromVec(*large_int_vec.get());
  }
}
BENCHMARK(bench_tree_from_shuffled_vec);

static void bench_tree_from_reversed_vec(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(1000);
  reverse(large_int_vec->begin(), large_int_vec->end());
  auto root = std::make_unique<Tree::Node<int>>(-1);
  for (auto _ : state) {
    root = Tree::Node<int>::FromVec(*large_int_vec.get());
  }
}
BENCHMARK(bench_tree_from_reversed_vec);

static void bench_tree_contains(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(1000);
  random_shuffle(large_int_vec->begin(), large_int_vec->end());
  auto large_int_vec_2 = setup_large_int_vec(1000);
  random_shuffle(large_int_vec_2->begin(), large_int_vec_2->end());
  auto root = Tree::Node<int>::FromVec(*large_int_vec.get());
  auto cur_idx = 0;
  auto res = false;
  for (auto _ : state) {
    // [] alias for use with ->
    res = root->Contains(large_int_vec_2->at(cur_idx));
    cur_idx++;
    cur_idx %= large_int_vec_2->size();
  }
}
BENCHMARK(bench_tree_contains);

static void bench_vec_contains(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(1000);
  random_shuffle(large_int_vec->begin(), large_int_vec->end());
  auto large_int_vec_2 = setup_large_int_vec(1000);
  random_shuffle(large_int_vec_2->begin(), large_int_vec_2->end());
  auto cur_idx = 0;
  auto res = large_int_vec->end();
  for (auto _ : state) {
    // [] alias for use with ->
    res = std::find(large_int_vec->begin(), large_int_vec->end(),
                    large_int_vec_2->at(cur_idx));
    cur_idx++;
    cur_idx %= large_int_vec_2->size();
  }
}
BENCHMARK(bench_vec_contains);

static void bench_umap_contains(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(1000);
  random_shuffle(large_int_vec->begin(), large_int_vec->end());
  auto large_int_vec_2 = setup_large_int_vec(1000);
  random_shuffle(large_int_vec_2->begin(), large_int_vec_2->end());

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
  auto large_int_vec = setup_large_int_vec(1000);
  random_shuffle(large_int_vec->begin(), large_int_vec->end());
  auto large_int_vec_2 = setup_large_int_vec(1000);
  random_shuffle(large_int_vec_2->begin(), large_int_vec_2->end());

  auto cur_idx = 0;
  auto res = 0;
  auto sorted = false;
  for (auto _ : state) {
    if (!sorted) {
      std::sort(large_int_vec->begin(), large_int_vec->end()); // amortize sort
      sorted = true;
    }

    res = std::binary_search(large_int_vec->begin(), large_int_vec->end(),
                             large_int_vec_2->at(cur_idx));
    cur_idx++;
    cur_idx %= large_int_vec_2->size();
  }
}
BENCHMARK(bench_vec_bsearch);

static void bench_google_shs(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(1000);
  random_shuffle(large_int_vec->begin(), large_int_vec->end());
  auto large_int_vec_2 = setup_large_int_vec(1000);
  random_shuffle(large_int_vec_2->begin(), large_int_vec_2->end());

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

static void bench_google_sms(benchmark::State &state) {
  auto large_int_vec = setup_large_int_vec(1000);
  random_shuffle(large_int_vec->begin(), large_int_vec->end());
  auto large_int_vec_2 = setup_large_int_vec(1000);
  random_shuffle(large_int_vec_2->begin(), large_int_vec_2->end());

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
BENCHMARK(bench_google_sms);

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
