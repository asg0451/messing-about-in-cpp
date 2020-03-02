#include "async.h"
#include "common.h"
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include <future> // async

#include <curl/curl.h>

// https://gist.github.com/alghanmi/c5d7b761b2c9ab199157
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

std::string fetch(std::string url) {
  auto curl = curl_easy_init();
  if (!curl) {
    return 0;
  } // why?

  std::string read_buffer;

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
  auto res = curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  return read_buffer;
}

int main() {
  auto url = "https://jsonplaceholder.typicode.com/todos/1";

  // equivalent but to show lambdas
  auto future = std::async([](auto url) { return fetch(url); }, url);
  auto future2 = std::async(fetch, url);

  std::cout << "waiting.." << std::endl;
  auto res = future.get();   // blocks until ready
  auto res2 = future2.get(); // blocks until ready
  std::cout << res << std::endl;
  std::cout << res2 << std::endl;

  // auto res = fetch(url);
}
