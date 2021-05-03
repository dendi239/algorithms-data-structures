#pragma once

#include <algorithm>
#include <list>
#include <set>
#include <vector>

#include "graphs/capacity-map.hpp"
#include "graphs/flow.hpp"
#include "graphs/vertex-property-map.hpp"

namespace flows {

template <class Graph, class CapacityMap>
class PushRelabel {
 public:
  using ValueType = typename CapacityMapTraits<CapacityMap>::ValueType;
  using VertexType = typename graphs::GraphTraits<Graph>::VertexType;
  using EdgeType = typename graphs::GraphTraits<Graph>::EdgeType;
  using Flow = PreFlow<ValueType, Graph>;

  PushRelabel(const Graph &graph, const CapacityMap &capacity_map,
              VertexType source, VertexType sink)
      : graph_(graph), capacity_map_(capacity_map),
        source_(source), sink_(sink),
        flow_(graph), residual_capacity_map_(flow_, capacity_map),
        limit_(NumberVertices(graph)),
        buckets_(limit_),
        current_edges_(graph),
        labels_(graph) {

    // TODO: Add iteration over all graph nodes for convenience.
    for (int node = 0; node < NumberVertices(graph); ++node) {
      current_edges_[node] = OutgoingEdges(graph, node).begin();
    }

    labels_[source] = NumberVertices(graph);
    for (auto edge : OutgoingEdges(graph, source)) {
      flow_(edge) += capacity_map(edge);
    }

    // TODO: Add iteration over all graph nodes for convenience.
    for (int node = 0; node < NumberVertices(graph); ++node) {
      if (node != source && node != sink) {
        buckets_[0].AddNode(node, IsActive(node));
      }
    }
  }

  void Push(EdgeType edge) {
    const EdgeType from = GetTarget(graph_, GetReversed(graph_, edge));
    const EdgeType to = GetTarget(graph_, edge);
    const ValueType diff = std::min(
        -flow_.Divergence(from), residual_capacity_map_(edge));

    bool inactive = !IsActive(to);
    flow_(edge) += diff;

    if (IsActive(to) && inactive && Get(labels_, to) < limit_ && to != sink_) {
      buckets_[labels_[to]].inactive_nodes -= 1;
      buckets_[labels_[to]].active_nodes.push_back(to);
    }
  }

  void Relabel(VertexType vertex) {
    const auto &edges = OutgoingEdges(graph_, vertex);
    bool found = false;

    for (auto it = edges.begin(); it != edges.end(); ++it) {
      auto target = GetTarget(graph_, *it);
      if (residual_capacity_map_(*it) > 0) {
        if (!found || labels_[vertex] > labels_[target] + 1) {
          found = true;
          labels_[vertex] = labels_[target] + 1;
          current_edges_[vertex] = it;
        }
      }
    }
  }

  void Discharge(VertexType vertex) {
    const auto &edges = OutgoingEdges(graph_, vertex);

    for (auto &it = At(current_edges_, vertex); it != edges.end(); ++it) {
      auto target = GetTarget(graph_, *it);
      if (IsAdmissible(vertex, target)) {
        Push(*it);
      }
      if (!IsActive(vertex)) {
        break;
      }
    }

    if (IsActive(vertex)) {
      Relabel(vertex);
    }
  }

  bool PushNode() {
    int level = max_label_;
    for (auto it = buckets_[level].active_nodes.begin();
         it != buckets_[level].active_nodes.end();) {
      VertexType vertex = *it;
      buckets_[level].active_nodes.erase(it++);
      Discharge(vertex);

      int vertex_label = Get(labels_, vertex);
      if (vertex_label != level) {
        if (vertex_label < limit_ && vertex != sink_ && vertex != source_) {
          buckets_[vertex_label].AddNode(vertex, flow_.Divergence(vertex) < 0);
        }
      } else {
        buckets_[level].inactive_nodes += 1;
      }

      if (IsActive(vertex) && vertex_label < limit_) {
        max_label_ = std::max(max_label_, vertex_label);
      }

      return true;
    }

    for (; level >= 0; --level) {
      if (!buckets_[level].active_nodes.empty()) {
        max_label_ = level;
        return true;
      }
    }

    return false;
  }

  Flow GetPreflow() const { return flow_; }

 private:
  using EdgeIt = typename graphs::GraphTraits<Graph>::EdgeIterator;

  struct Level {
    std::list<VertexType> active_nodes;
    int inactive_nodes = 0;

    bool Empty() const {
      return active_nodes.empty() && inactive_nodes == 0;
    }

    void AddNode(VertexType node, bool active) {
      if (active) {
        active_nodes.push_back(node);
      } else {
        ++inactive_nodes;
      }
    }
  };

  bool IsActive(VertexType vertex) const {
    return flow_.Divergence(vertex) < 0;
  }

  bool IsAdmissible(VertexType from, VertexType to) const {
    return Get(labels_, from) == Get(labels_, to) + 1;
  }

  int max_label_ = 0, limit_ = 0;

  const Graph &graph_;
  const CapacityMap &capacity_map_;

  const VertexType source_, sink_;

  Flow flow_;
  ResidualCapacityMap<Flow, CapacityMap> residual_capacity_map_;

  std::vector<Level> buckets_;
  graphs::VertexPropertyMap<Graph, EdgeIt> current_edges_;
  graphs::VertexPropertyMap<Graph, int> labels_;
};

template <class Graph, class CapacityMap>
auto MaxFlow(
    const Graph &graph, const CapacityMap &capacity_map,
    int source, int sink) {
  auto push_relabel = PushRelabel(graph, capacity_map, source, sink);
  while (push_relabel.PushNode()) {}
  auto preflow = push_relabel.GetPreflow();
  return preflow;
}

}  // namespace flows
