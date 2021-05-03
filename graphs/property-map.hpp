#pragma once

#include <utility>

#include "graphs/graph.hpp"

namespace graphs::detail {

/**
 * PropertyMap is base class for any property map assigned to some entities, but
 * stored outside.
 *
 * Basic examples for property maps are VertexPropertyMap and EdgePropertyMap.
 * Usually, these classes exist to adopt ctor to use more efficient vector
 * instead of std::unordered_map.
 *
 * This particular implementation works with container that support operator[]
 * with Vertex as a parameter.
 *
 * PropertyMap defines three functions to be used to manipulate data:
 * - const ValueType &Get(const PropertyMap &, const Key &key)
 * - void Set(PropertyMap &, const Key &key, const Value &value)
 * - ValueType &At(PropertyMap &, const Key &key)
 *
 * Note that many of algorithms use contract described above, so you can make
 * your own property map that capable of these three methods.
 */
template <class Key, class Value, class Container>
class PropertyMap {
 public:
  using KeyType = Key;
  using ValueType = Value;

  template <class ...Args>
  explicit PropertyMap(Args &&...args)
    : container_(std::forward<Args>(args)...) {}

  friend const ValueType &Get(const PropertyMap &map, const KeyType &key) {
    return map.container_[key];
  }

  friend void Set(PropertyMap &map, const KeyType &key, const ValueType &value) {
    map.container_[key] = value;
  }

  friend ValueType &At(PropertyMap &map, const KeyType &key) {
    return map.container_[key];
  }

  const ValueType &operator[](const KeyType &key) const {
    return container_[key];
  }

  ValueType &operator[](const KeyType &key) {
    return container_[key];
  }

 protected:
  Container container_;
};

}  // namespace graphs::detail
