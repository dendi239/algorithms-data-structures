#pragma once

#include <vector>

template <class T>
struct C {
  std::vector<T> facts, inv_facts;

  explicit C(size_t size) : facts(size, T(1)), inv_facts(size, T(1)) {
    for (size_t i = 1; i < size; ++i) {
      facts[i] = facts[i - 1] * T(i);
    }

    inv_facts[size - 1] = T(1) / facts[size - 1];
    for (size_t i = size - 1; i > 0; --i) {
      inv_facts[i - 1] = inv_facts[i] * T(i);
    }
  }

  auto operator()(int64_t n, int64_t k) -> T {
    if (k > n) {
      return T(0);
    }
    return facts[n] * inv_facts[k] * inv_facts[n - k];
  }
};