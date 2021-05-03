#pragma once

#include <unordered_map>
#include <vector>

#include "graphs/property-map.hpp"

namespace graphs {

template <class Graph, class ValueType, class = void>
class VertexPropertyMap : public detail::PropertyMap<
  typename graphs::GraphTraits<Graph>::VertexType, ValueType,
  std::unordered_map<typename GraphTraits<Graph>::VertexType, ValueType>> {
 private:
  using Super = detail::PropertyMap<
    typename GraphTraits<Graph>::VertexType, ValueType,
    std::unordered_map<typename GraphTraits<Graph>::VertexType, ValueType>>;

 public:
  explicit VertexPropertyMap(const Graph &graph) : Super() {}
};

template <class Graph, class ValueType>
class VertexPropertyMap<
  Graph, ValueType,
  typename std::enable_if<GraphTraits<Graph>::IsFlatVertices>::type>
  : public detail::PropertyMap<typename GraphTraits<Graph>::VertexType,
                               ValueType, std::vector<ValueType>> {
 private:
  using Super = detail::PropertyMap<typename GraphTraits<Graph>::VertexType,
                                    ValueType, std::vector<ValueType>>;

 public:
  explicit VertexPropertyMap(const Graph &graph) : Super(NumberVertices(graph)) {}
};

}  // namespace graphs
