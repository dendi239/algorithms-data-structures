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

#include <functional>

namespace segment_tree {

struct Defer {
  std::function<void()> func;

  template <class Func>
  Defer(Func func) : func(func) {}

  ~Defer() noexcept {
    func();
  }
};

template <class NodeReference>
class Walker {
 public:
  using Data = typename NodeReference::Data;
  using Monoid = typename NodeReference::Monoid;

  NodeReference node;
  int left, right;

  Walker() = default;
  Walker(NodeReference node, int left, int right)
      : node(node), left(left), right(right) {}

  int mid() const { return (left + right) / 2; }
  int size() const { return right - left; }

  bool disjoint(int begin, int end) const {
    return right <= begin || end <= left;
  }

  bool inside(int begin, int end) const {
    return begin <= left && right <= end;
  }

  Walker Left() { return {node.Left(), left, mid()}; }
  Walker Right() { return {node.Right(), mid(), right}; }

  Data &Get() { return node.Get(); }
  Data &Update() {
    return node.Get() = Monoid::Merge(Left().Get(), Right().Get());
  }

  auto DeferredUpdate() {
    return Defer{[this] { Update(); }};
  }
};

template <class Monoid>
struct RangeQuery {
  int begin, end;
  Monoid monoid;

  template <class Walker>
  auto perform(Walker walker) {
    if (walker.inside(begin, end)) {
      return monoid.Pure(walker.Get());
    }

    auto left = walker.Left();
    auto right = walker.Right();

    walker.DeferredUpdate();

    if (left.disjoint(begin, end)) {
      return perform(right);
    }
    if (right.disjoint(begin, end)) {
      return perform(left);
    }
    return monoid.Merge(perform(left), perform(right));
  }
};

} // namespace segment_tree
