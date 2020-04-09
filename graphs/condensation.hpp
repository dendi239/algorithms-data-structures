#pragma once

#include <vector>

#include <data-structures/partition.hpp>
#include <graphs/graph.hpp>

struct Condensation {
  Graph graph;
  Partition components;
};

auto BuildCondensation(const Graph &graph) -> Condensation;

auto ReversedGraph(const Graph &graph) -> Graph;
auto DfsExitOrder(const Graph &graph) -> std::vector<int>;

auto BuildCondensation(const Graph &graph) -> Condensation {
  auto order = DfsExitOrder(graph);
  auto reversed_graph = ReversedGraph(graph);

  auto partition_builder = PartitionBuilder(graph.Size());
  LambdaVisitor()
      .OnStart([&](int){ partition_builder.CreateNewGroup(); })
      .OnFinish([&](int node) { partition_builder.AddElementToLastGroup(node); })
      .Visit(reversed_graph, order);

  auto partition = partition_builder.Build();

  size_t condensation_size = partition.GroupsNumber();
  auto unique_condensation_links = std::vector<std::unordered_set<size_t>>(condensation_size);

  for (size_t node = 0; node < graph.Size(); ++node) {
    size_t from = partition.GroupId(node);
    for (size_t link : graph.Links(node)) {
      size_t to = partition.GroupId(link);
      if (from != to) {
        unique_condensation_links[from].insert(to);
      }
    }
  }

  auto condensation = Graph(condensation_size);
  for (size_t node = 0; node < condensation_size; ++node) {
    for (size_t link : unique_condensation_links[node]) {
      condensation.Connect(node, link);
    }
  }

  return {condensation, partition};
}

auto DfsExitOrder(const Graph &graph) -> std::vector<int> {
  std::vector<int> order;
  LambdaVisitor()
      .OnFinish([&order](int node) { order.push_back(node); })
      .Visit(graph);
  std::reverse(order.begin(), order.end());
  return order;
}

auto ReversedGraph(const Graph &graph) -> Graph {
  auto reversed_graph = Graph(graph.Size());
  for (size_t node = 0; node < reversed_graph.Size(); ++node) {
    for (auto link : graph.Links(node)) {
      reversed_graph.Connect(link, node);
    }
  }
  return reversed_graph;
}