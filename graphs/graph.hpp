#pragma once

#include <vector>

#include "utility/type_traits.hpp"

namespace graphs {

/**
 * This is documentation for **graph concept**.
 *
 * Generally, Graph is a pair (V, E) where V -- set of vertices and E is set of
 * edges. Each edge connects two vertices.
 *
 * Here's types you'll operate:
 *  Graph  -- type of graph itself
 *  Vertex -- type of vertex
 *  Edge   -- type of edge
 *  Edges  -- iterable over edges returned by OutgoingEdges
 *
 * Here's functions you'll operate:
 *  Edges  OutgoingEdges(const Graph &graph, Vertex vertex);
 *  Vertex GetTarget(const Graph &graph, Edge edge);
 *
 * We're using function-overload mechanism for polymorphic Graph structures
 * such as std::vector<std::vector<int>>.
 *
 * If you have **bidirectional graph**, then you'll have additionally:
 *  Edge GetReversed(const Graph &graph, Edge edge);
 *
 * You may have **flat vertices graph**, it operates with vertices as indices,
 * so you're allowed to use some flat collection. To know how big collection you
 * actually need, you can use following:
 *
 *  int NumberVertices(const Graph &graph);
 *
 * You may have **flat edge graph**, it operate edges just indices:
 * they are numbered from 0 to |E|-1 and it can be used to store some additional
 * properties of those: see flat::Graph, flat::EdgePropertiesMap.
 * In addition to previous methods, **flat graph** will have:
 *
 *  int NumberEdges(const Graph &graph);
 */

namespace detail {

template <class Graph, class = void>
struct TestFlatVertices : std::false_type {};

template <class Graph>
struct TestFlatVertices<
    Graph,
    decltype(NumberVertices(std::declval<const Graph &>()))> : std::true_type {
};

template <class Graph, class = void>
struct TestFlatEdges : std::false_type {};

template <class Graph>
struct TestFlatEdges<
    Graph,
    decltype(NumberEdges(std::declval<const Graph &>()))> : std::true_type {
};

}  // namespace detail

template <class Graph>
struct GraphTraits {
  using VertexType = decltype(GetTarget(
      std::declval<const Graph &>(),
      std::declval<type_traits::Placeholder>()));

  using EdgesType = decltype(OutgoingEdges(
      std::declval<const Graph &>(),
      std::declval<type_traits::Placeholder>()));

  using EdgeType = type_traits::RemoveCVRefT<decltype(*std::begin(std::declval<EdgesType>()))>;
  using EdgeIterator = type_traits::RemoveCVRefT<decltype(std::begin(std::declval<EdgesType>()))>;

  constexpr static bool IsFlatVertices = detail::TestFlatVertices<Graph, int>::value;
  constexpr static bool IsFlatEdges = detail::TestFlatEdges<Graph, int>::value;
};

}  // namespace graphs

/**
 * Pure std::vector<std::vector<int>> is a common way to represent graph, so
 * we might need to adopt graph concept with flat-vertices-graph concept as well
 *
 * Extending namespace std is necessary due to ADL: otherwise, these functions
 * are not visible from non-global namespace.
 */
namespace std {

int GetTarget(const vector<vector<int>> &graph, int edge) {
  return edge;
}

int NumberVertices(const vector<vector<int>> &graph) {
  return static_cast<int>(graph.size());
}

const auto &OutgoingEdges(const vector<vector<int>> &graph, int node) {
  return graph[node];
}

}  // namespace std
