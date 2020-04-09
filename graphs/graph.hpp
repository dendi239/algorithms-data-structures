#include <cstdint>
#include <vector>

int GetTarget(const std::vector<std::vector<int>> &graph, int edge) {
  return edge;
}

const auto &OutgoingEdges(const std::vector<std::vector<int>> &graph, int node) {
  return graph[node];
}
