#pragma once

#include <vector>

#include <unordered_map>
#include <vector>

#include "graphs/graph.hpp"
#include "graphs/property-map.hpp"

namespace graphs {

template <class Graph, class ValueType, class = void>
class EdgePropertyMap : public detail::PropertyMap<
  typename GraphTraits<Graph>::EdgeType, ValueType,
  std::unordered_map<typename GraphTraits<Graph>::VertexType, ValueType>> {
 private:
  using Super = detail::PropertyMap<
    typename GraphTraits<Graph>::VertexType, ValueType,
    std::unordered_map<typename GraphTraits<Graph>::VertexType, ValueType>>;

 public:
  explicit EdgePropertyMap(const Graph &graph) : Super() {}
};

template <class Graph, class ValueType>
class EdgePropertyMap<
  Graph, ValueType,
  typename std::enable_if<GraphTraits<Graph>::IsFlatEdges>::type>
  : public detail::PropertyMap<typename GraphTraits<Graph>::EdgeType,
                               ValueType, std::vector<ValueType>> {
 private:
  using Super = detail::PropertyMap<typename GraphTraits<Graph>::EdgeType,
                                    ValueType, std::vector<ValueType>>;

 public:
  explicit EdgePropertyMap(const Graph &graph) : Super(NumberEdges(graph)) {}
};

}  // namespace graphs
