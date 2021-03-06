//
// Copyright (c) 2020 Denys Smirnov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

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
      : data_(std::distance(begin, end)),
        logs_(std::distance(begin, end) + 1),
        less_{less} {
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
          auto new_value = std::min(
              data_[index].back(),
              data_[index + half].back(),
              less_);
          data_[index].push_back(new_value);
        } else {
          data_[index].push_back(data_[index].back());
        }
      }
    }
  }

  template <class Container>
  Rmq(const Container &container, Less less = {})
      : Rmq(container.begin(), container.end(), less)
  {}

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

namespace detail {

template <class Iterator>
using iterator_value_t = typename std::iterator_traits<Iterator>::value_type;

template <class Container>
using container_value_t = iterator_value_t<decltype(std::declval<Container>().begin())>;

} // namespace detail

template <class It, class Less = std::less<detail::iterator_value_t<It>>>
Rmq(It begin, It end, Less less = {})
  -> Rmq<detail::iterator_value_t<It>, Less>;

template <class Container, class Less = std::less<detail::container_value_t<Container>>>
Rmq(const Container &container, Less less = {})
  -> Rmq<detail::container_value_t<Container>, Less>;

} // namespace rmq
