#pragma once

#include <string_view>
#include <vector>

#include "../strings/suff-array.hpp"
#include "../data-structures/rmq.hpp"

class Lcp {
 public:
  static Lcp Build(std::string_view text);

  Lcp(std::vector<int> suffix_array, std::vector<int> lcp_array)
      : text_size_{static_cast<int>(suffix_array.size())},
        suffix_array_{std::move(suffix_array)},
        ranks_array_(text_size_),
        lcp_array_{std::move(lcp_array)},
        lcp_rmq_{make_rmq(lcp_array_.begin(), lcp_array_.end())} {
    for (int index = 0; index < text_size_; ++index) {
      ranks_array_[suffix_array_[index]] = index;
    }
  }

  int rank(int index) const { return ranks_array_[index]; }

  int lcp_rank(int lhs, int rhs) const {
    if (lhs > rhs) std::swap(lhs, rhs);
    return lcp_rmq_.closed(lhs, --rhs);
  }

  int lcp(int lhs, int rhs) const {
    if (lhs == rhs) return text_size_ - lhs;
    return lcp_rank(lhs, rhs);
  }

 private:
  const int text_size_;
  std::vector<int> suffix_array_;
  std::vector<int> ranks_array_;
  std::vector<int> lcp_array_;
  Rmq<int> lcp_rmq_;
};

Lcp Lcp::Build(std::string_view text) {
  auto[suffix_array, lcp_array] = SuffArray(text);
  return Lcp{suffix_array, lcp_array};
}
