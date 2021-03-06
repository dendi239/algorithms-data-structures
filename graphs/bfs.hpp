#pragma once

#include <queue>
#include <unordered_set>

namespace traverses {

template <class Vertex, class Edge>
class BfsVisitor {
public:
    virtual void DiscoverVertex(Vertex /*vertex*/) {}
    virtual void ExamineEdge(const Edge & /*edge*/) {}
    virtual void ExamineVertex(Vertex /*vertex*/) {}
    virtual ~BfsVisitor() = default;
};

template <class Vertex, class Graph, class Visitor>
void BreadthFirstSearch(Vertex node, const Graph &graph, Visitor visitor);

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
