#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <data-structures/partition.hpp>
#include <graphs/visitor.hpp>
#include <graphs/graph.hpp>

namespace condensation {

using Graph = std::vector<std::vector<int>>;

struct Condensation {
  Graph graph;
  Partition components;
};

namespace detail {

template <class Graph>
auto DfsExitOrder(const Graph &graph) -> std::vector<int> {
  std::vector<int> order;
  traverses::DepthFirstSearch(graph, traverses::Visitor()
      .OnFinish([&order](int node) { order.push_back(node); }));
  std::reverse(order.begin(), order.end());
  return order;
}

auto ReversedGraph(const Graph &graph) -> Graph {
  auto reversed_graph = Graph(graph.size());
  for (size_t node = 0; node < reversed_graph.size(); ++node) {
    for (auto link : OutgoingEdges(graph, node)) {
      Connect(&reversed_graph, link, node);
    }
  }
  return reversed_graph;
}

}  // namespace detail

template<class Graph>
auto BuildCondensation(const Graph &graph) -> Condensation {
  auto order = detail::DfsExitOrder(graph);
  auto reversed_graph = detail::ReversedGraph(graph);

  auto partition_builder = PartitionBuilder(graph.size());
  traverses::DepthFirstSearch(reversed_graph, order, traverses::Visitor()
      .OnStart([&partition_builder](int) { partition_builder.CreateNewGroup(); })
      .OnFinish([&partition_builder](int node) { partition_builder.AddElementToLastGroup(node); }));

  auto partition = partition_builder.Build();

  size_t condensation_size = partition.GroupsNumber();
  auto unique_condensation_links = std::vector<std::unordered_set<size_t>>(condensation_size);

  for (size_t node = 0; node < graph.size(); ++node) {
    size_t from = partition.GroupId(node);
    for (size_t link : OutgoingEdges(graph, node)) {
      size_t to = partition.GroupId(link);
      if (from != to) {
        unique_condensation_links[from].insert(to);
      }
    }
  }

  auto condensation = Graph(condensation_size);
  for (size_t node = 0; node < condensation_size; ++node) {
    for (size_t link : unique_condensation_links[node]) {
      Connect(&condensation, node, link);
    }
  }

  return {condensation, partition};
}

}  // namespace condensation
