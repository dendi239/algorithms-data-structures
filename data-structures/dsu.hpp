#include <numeric>
#include <vector>

class Dsu {
 public:
  explicit Dsu(size_t size) : parents_(size), ranks_(size) {
    std::iota(parents_.begin(), parents_.end(), 0);
  }

  int parent(int node) const
  { return parents_[node] == node ? node : parents_[node] = parent(parents_[node]); }

  bool is_same(int first, int second) const
  { return parent(first) == parent(second); }

  void merge(int first, int second) {
    first = parent(first), second = parent(second);
    if (first == second) return;
    if (ranks_[first] < ranks_[second]) std::swap(first, second);

    parents_[second] = first;
    ranks_[first] += ranks_[first] == ranks_[second];
  }

 private:
  mutable std::vector<int> parents_;
  std::vector<int> ranks_;
};
