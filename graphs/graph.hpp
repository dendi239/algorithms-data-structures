#pragma once

#include <vector>

namespace std {

int GetTarget(const vector<vector<int>> &graph, int edge) {
  return edge;
}

int NumberVertices(const vector<vector<int>> &graph) {
  return graph.size();
}

const auto &OutgoingEdges(const vector<vector<int>> &graph, int node) {
  return graph[node];
}

void Connect(vector<vector<int>> *graph, int from, int to) {
  (*graph)[from].push_back(to);
}

void BiConnect(vector<vector<int>> *graph, int first, int second) {
  (*graph)[first].push_back(second);
  (*graph)[second].push_back(first);
}

}  // namespace std
