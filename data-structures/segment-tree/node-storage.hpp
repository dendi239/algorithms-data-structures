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

#include <vector>

namespace segment_tree {

template <class MonoidType, class StoredType = decltype(MonoidType::Pure(int{}))>
class VectorStorage {
 public:
  using Data = StoredType;
  using Monoid = MonoidType;

  explicit VectorStorage(int /*size*/) : storage(1) {}

  class NodeReference {
   public:
    using Data = StoredType;
    using Monoid = MonoidType;

    Data &Get();

    NodeReference Left();
    NodeReference Right();

   private:
    friend class VectorStorage;

    NodeReference(VectorStorage *storage, int index)
        : storage(storage),
          index(index)
    {}

    auto &Node() { return storage->storage[index]; }

    VectorStorage *storage;
    int index;
  };

  NodeReference Root() { return NodeReference{this, 0}; }

 private:
  struct Node {
    Data data;
    int left = -1, right = -1;
  };

  int Son(int &node_holder) {
    if (node_holder != -1) {
      return node_holder;
    }

    node_holder = static_cast<int>(storage.size());
    int new_id = node_holder;
    storage.push_back(Node{});
    return new_id;
  }

  std::vector<Node> storage;
};

template<class Monoid, class Stored>
auto VectorStorage<Monoid, Stored>::NodeReference::Get() -> Data & {
  return storage->storage[index].data;
}

template<class Monoid, class Stored>
auto VectorStorage<Monoid, Stored>::NodeReference::Left() -> NodeReference {
  return {
      storage,
      storage->Son(Node().left),
  };
}

template<class Monoid, class Stored>
auto VectorStorage<Monoid, Stored>::NodeReference::Right() -> NodeReference {
  return {
      storage,
      storage->Son(Node().right),
  };
}

} // namespace segment_tree
