#pragma once

#include <queue>

#include <graphs/vertex-property-map.hpp>

namespace traverses {

template <class Vertex, class Edge>
class BfsVisitor {
 public:
  virtual void DiscoverVertex(Vertex /*vertex*/) {}
  virtual void ExamineEdge(const Edge & /*edge*/) {}
  virtual void ExamineVertex(Vertex /*vertex*/) {}
  virtual ~BfsVisitor() = default;
};

template <class Graph, class Visitor,
    class Vertex = typename graphs::GraphTraits<Graph>::Node>
void BreadthFirstSearch(Vertex origin_vertex, const Graph &graph,
                        Visitor visitor) {
  enum class Status {
    NEW, LOOKED,
  };

  graphs::VertexPropertyMap<Graph, Status> discovered_nodes(graph);
  std::queue<Vertex> queue;

  auto discover = [&discovered_nodes, &queue, &visitor](Vertex node) {
    if (discovered_nodes[node] == Status::LOOKED) return;
    discovered_nodes[node] = Status::LOOKED;
    queue.push(node);
    visitor.DiscoverVertex(node);
  };

  for (discover(origin_vertex); !queue.empty(); queue.pop()) {
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
