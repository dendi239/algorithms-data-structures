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

#include <numeric>
#include <vector>

class Dsu {
 public:
  explicit Dsu(size_t size) : parents_(size), ranks_(size) {
    std::iota(parents_.begin(), parents_.end(), 0);
  }

  int parent(int node) const {
    return parents_[node] == node
           ? node
           : parents_[node] = parent(parents_[node]);
  }

  bool is_same(int first, int second) const {
    return parent(first) == parent(second);
  }

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
