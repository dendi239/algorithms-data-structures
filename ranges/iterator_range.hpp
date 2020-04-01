#pragma once

#include <iterator>
#include <utility>

template <class Iterator>
class IteratorRange {
 public:
  IteratorRange(Iterator begin, Iterator end)
      : begin_{std::move(begin)}, end_{std::move(end)}
  {}

  auto begin() const { return begin_; }
  auto end() const { return end_; }

 private:
  Iterator begin_, end_;
};
