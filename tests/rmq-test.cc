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

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../data-structures/rmq.hpp"

template <class T>
struct SimpleRmq {
  std::vector<T> xs;

  explicit SimpleRmq(const std::vector<T> &xs) : xs(xs) {}

  T operator()(int begin, int end) const {
    T res = xs[begin];
    for (int i = begin + 1; i < end; ++i) {
      res = std::min(res, xs[i]);
    }
    return res;
  }
};

TEST_CASE("Sample test") {
  auto vec = std::vector<int>{
    1, 3, 2, 5, 0,
  };

  auto rmq1 = rmq::Rmq(vec);
  auto rmq2 = rmq::Rmq(vec.begin(), vec.end());
  auto expected_rmq = SimpleRmq(vec);

  for (int i = 0; i < vec.size(); ++i) {
    for (int j = i + 1; j <= vec.size(); ++j) {
      auto expected = expected_rmq(i, j);
      REQUIRE(rmq1.min(i, j) == expected);
      REQUIRE(rmq2.min(i, j) == expected);
    }
  }
}
