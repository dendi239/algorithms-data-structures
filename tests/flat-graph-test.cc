#include <catch2/catch_all.hpp>

#include <algorithm>

#include <graphs/graph.hpp>
#include <graphs/flat-graph.hpp>

TEST_CASE("Sample") {
  using Graph = std::vector<std::vector<int>>;

  for (auto [graph, expected] : std::vector<std::pair<Graph, Graph>>{
      {{{1},    {2},    {3},  {}},
       {{1}, {0, 2}, {1, 3}, {2}}},
  }) {
    std::vector<std::vector<int>> actual(graph.size());
    auto flat_graph = flat::Graph(graph);

    for (int node = 0; node < NumberVertices(flat_graph); ++node) {
      for (auto edge : OutgoingEdges(flat_graph, node)) {
        actual[node].push_back(GetTarget(flat_graph, edge));
      }
      std::sort(actual[node].begin(), actual[node].end());
    }

    REQUIRE(expected == actual);
  }
}
