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

#include "../data-structures/node-storage.hpp"
#include "../data-structures/walker.hpp"

struct SumData {
  int sum = 0;

  static SumData Merge(SumData lhs, SumData rhs) {
    return {lhs.sum + rhs.sum};
  }
};

using Storage = VectorStorage<SumData>;
using NodeReference = Storage::NodeReference;
using SumWalker = Walker<NodeReference>;

struct IotaBuilder {
  template<class Walker>
  void build(Walker walker) {
    if (walker.size() == 1) {
      walker.Get().sum = walker.left;
      return;
    }

    build(walker.Left()), build(walker.Right());
    walker.Update();
  }
};

struct Getter {
  int left, right;

  template <class Walker>
  int Get(Walker walker) {
    if (walker.inside(left, right)) {
      return walker.Get().sum;
    }
    if (walker.disjoint(left, right)) {
      return 0;
    }

    return Get(walker.Left()) + Get(walker.Right());
  }
};

template<class ...Args>
struct Combine : Args... {
  using Args::operator()...;
};

template<class ...Args>
Combine(Args...) -> Combine<Args...>;

TEST_CASE("Sample test") {
  auto stree = STree<Storage>(10);
  IotaBuilder{}.build(stree.Root());

  auto Get = Combine {
    [](auto &&stree, int index) {
      return Getter{index, index + 1}.Get(stree.Root());
    },
    [](auto &&stree, int begin, int end) {
      return Getter{begin, end}.Get(stree.Root());
    },
  };

  for (int i = 0; i < 10; ++i) {
    REQUIRE(Get(stree, i) == i);
  }

  for (int l = 0; l < 10; ++l) {
    int expected = 0;
    for (int r = l; r <= 10; expected += r++) {
      REQUIRE(expected == Get(stree, l, r));
    }
  }
}
