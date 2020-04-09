#pragma once

#include <queue>
#include <unordered_set>

namespace traverses {

template <class NodeT, class EdgeT>
class BfsVisitor {
 public:
  using NodeType = NodeT;
  using EdgeType = EdgeT;

  virtual void DiscoverVertex(NodeType vertex) { /* no-op */ }
  virtual void ExamineEdge(const EdgeType& edge) { /* no-op */ }
  virtual void ExamineVertex(NodeType vertex) { /* no-op */ }
};

template <class NodeType, class Graph, class Visitor>
void BreadthFirstSearch(NodeType node, const Graph &graph, Visitor visitor);

}  // namespace traverses

namespace traverses {

template <class NodeType, class Graph, class Visitor>
void BreadthFirstSearch(NodeType node, const Graph &graph, Visitor visitor) {
  std::unordered_set<NodeType> discovered_nodes;
  std::queue<NodeType> queue;

  auto discover = [&discovered_nodes, &queue, &visitor](NodeType node) {
    if (discovered_nodes.count(node)) return;
    discovered_nodes.insert(node);
    queue.push(node);
    visitor.DiscoverVertex(node);
  };

  for (discover(node); !queue.empty(); queue.pop()) {
    auto head = queue.front();
    visitor.ExamineVertex(head);

    for (const auto &edge : OutgoingEdges(graph, head)) {
      visitor.ExamineEdge(edge);

      auto target = GetTarget(graph, edge);
      discover(target);
    }
  }
}

}  // namespace traverses
