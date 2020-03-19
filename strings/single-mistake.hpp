#pragma once

#include <algorithm>
#include <vector>

#include <strings/z-function.hpp>

template <class Char>
class OneMistakeFinder {
 public:
  template <class Text, class Pattern>
  OneMistakeFinder(const Text &text, const Pattern &pattern)
      : pattern_size(static_cast<int>(pattern.size()))
      , text_size(static_cast<int>(text.size())) {
    {
      std::vector<Char> straight(text.size() + pattern.size());
      auto it = std::copy(pattern.begin(), pattern.end(), straight.begin());
      std::copy(text.begin(), text.end(), it);
      z_straight_ = z_function(straight);
    }
    {
      std::vector<Char> inverted(text.size() + pattern.size());
      auto it = std::copy(pattern.rbegin(), pattern.rend(), inverted.begin());
      std::copy(text.rbegin(), text.rend(), it);
      z_inverted_ = z_function(inverted);
    }
  }

  int lcp(int text_start) const
  { return std::min(z_straight_[pattern_size + text_start], pattern_size); }

  int inv_lcp(int text_end) const {
    int inv_start = text_size - 1 - text_end;
    return std::min(pattern_size, z_inverted_[inv_start + pattern_size]);
  }

 private:
  const int pattern_size, text_size;
  std::vector<int> z_straight_, z_inverted_;
};

template <class Text, class Pattern>
auto FindMatchesWithMistake(const Text &text, const Pattern &pattern) {
  using Char = typename std::iterator_traits<decltype(std::declval<Text>().begin())>::value_type;
  auto finder = OneMistakeFinder<Char>(text, pattern);

  std::vector<int> matches;
  const int pattern_size = static_cast<int>(pattern.size());
  for (size_t first = 0, last = pattern.size() - 1; last < text.size();
       ++first, ++last) {
    if (finder.lcp(first) + 1 + finder.inv_lcp(last) >= pattern_size) {
      matches.push_back(first);
    }
  }

  return matches;
}