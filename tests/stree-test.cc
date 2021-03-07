#include <catch2/catch_all.hpp>

#include "../data-structures/segment-tree/segment-tree.hpp"

struct SumData {
  static auto Pure(int64_t value) {
    return value;
  }

  static auto Merge(int64_t lhs, int64_t rhs) -> int64_t {
    return lhs + rhs;
  }
};

TEST_CASE("Pure sumtree test") {
  std::vector<int64_t> xs(10);
  std::mt19937 rnd(239);
  for (auto &x : xs) {
    x = rnd();
  }

  auto stree = segment_tree::build<SumData>(xs);
  for (int l = 0; l < 10; ++l) {
    int64_t expected = xs[l];
    for (int r = l + 1; r <= 10; ++r) {
      REQUIRE(expected == stree.Fold(l, r));
      if (r < 10) {
        expected += xs[r];
      }
    }
  }
}

TEST_CASE("Iota builder test") {
  auto stree = segment_tree::build<SumData>(10, [](int index) {
    return index;
  });

  for (int i = 0; i < 10; ++i) {
    REQUIRE(stree.Fold(i, i+1) == i);
  }

  for (int l = 0; l < 10; ++l) {
    int64_t expected = l;
    for (int r = l + 1; r <= 10; expected += r++) {
      REQUIRE(stree.Fold(l, r) == expected);
    }
  }
}

template <class T>
struct AtLeastFinder {
  T value;

  template <class Walker>
  int Find(Walker w) {
    if (w.Get() < value) {
      return -1;
    }

    if (w.size() == 1) {
      return w.left;
    }

    int left = Find(w.Left());
    if (left != -1) {
      return left;
    } else {
      return Find(w.Right());
    }
  }
};

template <class T>
struct Setter {
  int index;
  T value;

  template <class Walker>
  void set(Walker walker) {
    if (walker.size() == 1) {
      walker.Get() = Walker::Monoid::Pure(value);
    } else {
      set(index < walker.mid() ? walker.Left() : walker.Right());
      walker.Update();
    }
  }
};

struct MaxData {
  template <class T>
  static auto Pure(T value) {
    return value;
  }

  template <class T>
  static auto Merge(T lhs, T rhs) {
    return std::max(lhs, rhs);
  }
};

TEST_CASE("Pure maxtree test") {
  auto stree = segment_tree::build<MaxData>(std::vector<int>{1, 3, 2, 4, 6});
  auto find = [&](int x) { return AtLeastFinder<int>{x}.Find(stree.Root()); };
  auto set = [&](int i, int x) { Setter<int>{i, x}.set(stree.Root()); };
  REQUIRE(find(2) == 1);
  REQUIRE(find(5) == 4);
  set(2, 5);
  REQUIRE(find(4) == 2);
  REQUIRE(find(8) == -1);
  set(3, 7);
  REQUIRE(find(6) == 3);
}
