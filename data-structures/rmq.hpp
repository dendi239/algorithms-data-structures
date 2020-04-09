#pragma once

#include <algorithm>
#include <functional>
#include <vector>

template <class T, class Less = std::less<T>>
class Rmq {
 public:
  template <class It>
  Rmq(It begin, It end, Less less = {})
      : data_(std::distance(begin, end))
      , logs_(std::distance(begin, end) + 1)
      , less_{less} {
    int len = std::distance(begin, end);

    for (int i = 0; i < len; ++i) {
      data_[i].push_back(*(begin + i));
    }

    for (int exp = 1, half = 1; (1 << exp) < (len << 1); ++exp, half <<= 1) {
      for (int diff = half; diff < (half << 1) && diff <= len; ++diff) {
        logs_[diff] = exp - 1;
      }
      for (int index = 0; index < len; ++index) {
        if (index + half < len) {
          auto new_value = std::min(data_[index].back(), data_[index + half].back(), less_);
          data_[index].push_back(new_value);
        } else {
          data_[index].push_back(data_[index].back());
        }
      }
    }
  }

  T min(size_t begin, size_t end) const {
    auto log = logs_[end - begin];
    auto lhs = data_[begin][log], rhs = data_[end - (1 << log)][log];
    return std::min(lhs, rhs, less_);
  }

  T closed(size_t lhs, size_t rhs) const {
    if (lhs > rhs) std::swap(lhs, rhs);
    return min(lhs, rhs + 1);
  }

 private:
  std::vector<std::vector<T>> data_;
  std::vector<int> logs_;
  Less less_;
};

template <class It, class Less = std::less<typename std::iterator_traits<It>::value_type>>
auto make_rmq(It begin, It end, Less less = {}) {
  return Rmq<typename std::iterator_traits<It>::value_type>(begin, end, less);
}
