#pragma once

#include <algorithm>
#include <functional>
#include <vector>

namespace rmq {

template <class T, class Less = std::less<T>>
class Rmq {
 public:
  template <class It>
  Rmq(It begin, It end, Less less = {})
      : data_(1),
        logs_(std::distance(begin, end) + 1),
        less_{less} {
    int len = std::distance(begin, end);

    for (int i = 0; i < len; ++i) {
      data_[0].push_back(*(begin + i));
    }

    for (int exp = 1, half = 1; (1 << exp) < (len << 1); ++exp, half <<= 1) {
      for (int diff = half; diff < (half << 1) && diff <= len; ++diff) {
        logs_[diff] = exp - 1;
      }
      int old = exp - 1;
      data_.emplace_back(len);

      for (int index = 0; index < len; ++index) {
        if (index + half < len) {
          auto new_value = std::min(
              data_[old][index],
              data_[old][index + half],
              less_);
          data_[exp][index] = new_value;
        } else {
          data_[exp][index] = data_[old][index];
        }
      }
    }
  }

  template <class Container>
  Rmq(const Container &container, Less less = {})
      : Rmq(container.begin(), container.end(), less) {}

  T min(size_t begin, size_t end) const {
    auto log = logs_[end - begin];
    auto lhs = data_[log][begin], rhs = data_[log][end - (1 << log)];
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

namespace detail {

template <class Iterator>
using iterator_value_t = typename std::iterator_traits<Iterator>::value_type;

template <class Container>
using container_value_t = iterator_value_t<decltype(std::declval<Container>().begin())>;

} // namespace detail

template <class It, class Less = std::less<detail::iterator_value_t<It>>>
Rmq(It begin, It end, Less less = {})
-> Rmq<detail::iterator_value_t<It>, Less>;

template <class Container, class Less = std::less<detail::container_value_t<
    Container>>>
Rmq(const Container &container, Less less = {})
-> Rmq<detail::container_value_t<Container>, Less>;

} // namespace rmq
