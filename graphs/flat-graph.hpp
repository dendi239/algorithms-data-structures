#pragma once

#include <algorithm>
#include <unordered_map>
#include <utility>

#include "graphs/graph.hpp"
#include "ranges/iterator_range.hpp"

namespace flat {

/**
 * flat::Graph maintains bidirectional graph with flat edges storage.
 *
 * flat::Graph satisfies Graph concept, flat-nodes-graph concept,
 * flat-edges-graph concept, and bidirectional graph concept, so it has friend
 * functions:
 * - auto OutgoingEdges(const Graph &graph, int node);
 * - int GetTarget(const Graph &graph, int edge);
 *
 * - int NumberVertices(const Graph &graph);
 * - int NumberEdges(const Graph &graph);
 *
 * - int GetReversed(const Graph &graph, int edge);
 *
 * Graph is a **flat graph**, so its nodes numbered from 0 to |V|-1, as well as
 * edges are numbered from 0 to |E|-1, so you're allowed to use flat properties
 * map using EdgePropertiesMap<T> class, or build your own.
 */
class Graph {
 public:
  struct Edge {
    int to;
    int rev_index;
  };

  template <class BaseGraph>
  explicit Graph(const BaseGraph &base_graph)
      : size_(NumberVertices(base_graph)), segment_(size_) {
    std::vector<std::unordered_map<int, int>> edges_enumerated(size_);

    for (int node = 0; node < size_; ++node) {
      for (auto edge : OutgoingEdges(base_graph, node)) {
        int target = GetTarget(base_graph, edge);
        if (edges_enumerated[node].count(target)) { continue; }

        int next_id = static_cast<int>(data_.size());
        data_.push_back({target, next_id + 1});
        data_.push_back({node, next_id});
        edges_enumerated[node][target] = next_id;
        edges_enumerated[target][node] = next_id + 1;
      }
    }

    for (int node = 0; node < size_; ++node) {
      segment_[node].len = static_cast<int>(edges_enumerated[node].size());
    }

    for (int node = 0; node + 1 < size_; ++node) {
      segment_[node + 1].start = segment_[node].start + segment_[node].len;
    }

    edge_indices_.resize(data_.size());
    std::vector<int> cursor(size_);
    for (int node = 0; node < size_; ++node) {
      for (auto[target, index] : edges_enumerated[node]) {
        edge_indices_[segment_[node].start + cursor[node]] = index;
        ++cursor[node];
      }
    }
  }

  friend int NumberVertices(const Graph &graph) {
    return graph.size_;
  }

  friend int NumberEdges(const Graph &graph) {
    return static_cast<int>(graph.data_.size());
  }

  friend auto OutgoingEdges(const Graph &graph, int node) {
    return IteratorRange{
      graph.edge_indices_.begin() + graph.segment_[node].start,
      graph.edge_indices_.begin() + graph.segment_[node].end(),
    };
  }

  friend int GetTarget(const Graph &graph, int edge_id) {
    return graph.data_[edge_id].to;
  }

  friend int GetReversed(const Graph &graph, int edge_id) {
    return graph.data_[edge_id].rev_index;
  }

 private:
  struct NodeInfo {
    int start, len;

    int end() const {
      return start + len;
    }
  };

  const int size_;
  std::vector<NodeInfo> segment_;
  std::vector<int> edge_indices_;
  std::vector<Edge> data_;
};

} // namespace flat
