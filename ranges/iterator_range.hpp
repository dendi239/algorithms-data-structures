#pragma once

#include <iterator>
#include <utility>

template <class Iterator>
class IteratorRange {
 public:
  IteratorRange(Iterator begin, Iterator end)
      : begin_{std::move(begin)}, end_{std::move(end)} {}

  template <class Container>
  explicit IteratorRange(Container &container)
      : begin_{std::begin(container)}, end_(std::end(container)) {}

  auto begin() const { return begin_; }
  auto end() const { return end_; }

 private:
  Iterator begin_, end_;
};

template <class Container>
IteratorRange(Container &container)
-> IteratorRange<decltype(std::begin(std::declval<Container>()))>;
