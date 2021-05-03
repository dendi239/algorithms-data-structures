#pragma once

#include <vector>

#include "graphs/edge-property-map.hpp"

namespace flows {

template <class CapMap>
struct CapacityMapTraits {
  using ValueType = typename CapMap::ValueType;
};

template <class T = int>
class CapacityMap {
 public:
  using ValueType = T;

  explicit CapacityMap(int vertex_count)
      : capacities_(vertex_count, std::vector<T>(vertex_count)) {}

  void AddEdge(int from, int to, T capacity) { capacities_[from][to] += capacity; }
  void SetEdge(int from, int to, T capacity) { capacities_[from][to] = capacity; }

  T operator()(int from, int to) const { return capacities_[from][to]; }

 private:
  std::vector<std::vector<T>> capacities_;
};

template<class Flow, class CapacityMap>
class ResidualCapacityMap {
 public:
  using ValueType = typename CapacityMapTraits<CapacityMap>::ValueType;

  ResidualCapacityMap(const Flow &flow, const CapacityMap &capacity_map)
      : flow_(flow), capacity_map_(capacity_map) {}

  ValueType operator()(int edge_id) const {
    auto capacity = capacity_map_(edge_id);
    auto flow = flow_(edge_id);
    return capacity - flow;
  }

 private:
  const Flow &flow_;
  const CapacityMap &capacity_map_;
};

}  // namespace flows

template <class Graph, class Capacity>
class CapacityMap : public graphs::EdgePropertyMap<Graph, Capacity> {
 public:
  using ValueType = Capacity;

  explicit CapacityMap(const Graph &bidirectional_graph)
    : graphs::EdgePropertyMap<Graph, Capacity>(bidirectional_graph) {}

  const Capacity &operator()(int edge_id) const {
    return Get(*this, edge_id);
  }

  Capacity &operator()(int edge_id) {
    return At(*this, edge_id);
  }
};
