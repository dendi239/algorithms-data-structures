#include <catch2/catch_all.hpp>

#include <iostream>
#include <vector>

#include <graphs/flat-graph.hpp>
#include <graphs/push-relabel.hpp>

using std::vector;

struct FlowNetwork {
  vector<vector<int>> capacity;
  int source, sink;
  int maxflow;

  vector<vector<int>> get_adj() const {
    int n = static_cast<int>(capacity.size());
    vector<vector<int>> adj(n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (capacity[i][j] || capacity[j][i])
          adj[i].push_back(j);
      }
    }
    return adj;
  }
};

vector<FlowNetwork> flow_networks = {
    // flow network from the e-maxx-eng article
    {{
         {0, 7, 0, 0, 4, 0},
         {0, 0, 5, 3, 0, 0},
         {0, 0, 0, 0, 0, 8},
         {0, 0, 3, 0, 0, 5},
         {0, 3, 0, 2, 0, 0},
         {0, 0, 0, 0, 0, 0}
     }, 0, 5, 10},
    // flow network from wikipedia (Edmonds-Karp article)
    {{
         {0, 3, 0, 3, 0, 0, 0},
         {0, 0, 4, 0, 0, 0, 0},
         {3, 0, 0, 1, 2, 0, 0},
         {0, 0, 0, 0, 2, 6, 0},
         {0, 1, 0, 0, 0, 0, 1},
         {0, 0, 0, 0, 0, 0, 9},
         {0, 0, 0, 0, 0, 0, 0}
     }, 0, 6, 5},
    // worst case flow network (for Ford-Fulkerson) from wikipedia
    {{
         {0, 1000, 1000, 0},
         {0, 0, 1, 1000},
         {0, 0, 0, 1000},
         {0, 0, 0, 0}
     }, 0, 3, 2000},
    // example from brilliant.org
    {{
         {0, 16, 0, 13, 0, 0},
         {0, 0, 12, 10, 0, 0},
         {0, 0, 0, 9, 0, 20},
         {0, 4, 0, 0, 14, 0},
         {0, 0, 7, 0, 0, 4},
         {0, 0, 0, 0, 0, 0}
     }, 0, 5, 23},
    {{
         {0, 10, 5, 15, 0, 0, 0, 0},
         {0, 0, 4, 0, 9, 15, 0, 0},
         {0, 0, 0, 4, 0, 8, 0, 0},
         {0, 0, 0, 0, 0, 0, 16, 0},
         {0, 0, 0, 0, 0, 15, 0, 10},
         {0, 0, 0, 0, 0, 0, 15, 10},
         {0, 0, 6, 0, 0, 0, 0, 10},
         {0, 0, 0, 0, 0, 0, 0, 0}
     }, 0, 7, 28},
    // example from https://web.stanford.edu/class/cs97si/08-network-flow-problems
    {{
         {0, 0, 10, 0, 0, 12},
         {0, 0, 0, 4, 0, 7},
         {4, 14, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {16, 0, 12, 0, 0, 0},
         {0, 0, 9, 20, 0, 0}
     }, 4, 3, 23},
};

TEST_CASE("Samples") {
  for (const auto &flow_network : flow_networks) {
    auto graph = flow_network.get_adj();
    auto flat_graph = flat::Graph(graph);
    auto flat_capacity_map = CapacityMap<flat::Graph, int>(flat_graph);

    for (int node = 0; node < NumberVertices(graph); ++node) {
      for (auto edge : OutgoingEdges(flat_graph, node)) {
        int target = GetTarget(flat_graph, edge);
        flat_capacity_map(edge) = flow_network.capacity[node][target];
      }
    }

    auto max_flow = flows::MaxFlow(flat_graph, flat_capacity_map, flow_network.source, flow_network.sink);
    INFO("Flow from " << flow_network.source << " to " << flow_network.sink);
    CHECK(-max_flow.Divergence(flow_network.sink) == flow_network.maxflow);
  }
}
