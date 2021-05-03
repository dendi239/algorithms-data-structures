#include <catch2/catch_all.hpp>
#include <utility>
#include <data-structures/rmq.hpp>

template <class T>
struct SimpleRmq {
  std::vector<T> xs;

  explicit SimpleRmq(std::vector<T> xs) : xs(std::move(xs)) {}

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
