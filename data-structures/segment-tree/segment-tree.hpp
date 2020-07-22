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

#pragma once

#include "node-storage.hpp"
#include "walker.hpp"

template<class Unary>
struct Builder {
  Unary unary;

  template<class Walker>
  void build(Walker walker) {
    if (walker.size() == 1) {
      walker.Get() = Walker::Monoid::Pure(unary(walker.left));
    } else {
      build(walker.Left()), build(walker.Right());
      walker.Update();
    }
  }
};

template<class Storage>
class STree {
 public:
  explicit STree(int size) : begin_{0}, end_{size} {}

  Walker<typename Storage::NodeReference> Root() {
    return Walker<typename Storage::NodeReference>{
        storage_.Root(),
        begin_, end_,
    };
  }

  template<class Unary>
  void Build(Unary unary) {
    Builder<Unary>{unary}
      .build(Root());
  }

  template<class T = typename Storage::Monoid, class ...Args>
  auto Fold(int begin, int end, Args &&...args) {
    return RangeQuery<T>{T{args...}, begin, end}
        .perform(Root());
  }

 private:
  Storage storage_;
  int begin_, end_;
};

template<class Monoid>
struct stree_builder {
  template<class Unary>
  auto operator()(int size, Unary unary) {
    using NodeType = decltype(std::declval<Unary>()(0));
    using Storage = VectorStorage<Monoid, NodeType>;

    auto stree = STree<Storage>(size);
    stree.Build(unary);
    return stree;
  }

  template<class T>
  auto operator()(const std::vector<T> &xs) {
    int size = static_cast<int>(xs.size());
    return (*this)(size, [&](int index) {
      return xs[index];
    });
  }
};

template<class Monoid>
stree_builder<Monoid> build;
