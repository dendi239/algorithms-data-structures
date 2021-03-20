#pragma once

#include <numeric>
#include <stack>
#include <vector>

namespace traverses {

template <class Graph, class Visitor, class NodeType>
void DepthFirstSearch(const Graph &graph, Visitor visitor, NodeType start);

template <class Graph, class Visitor, class It>
void DepthFirstSearch(const Graph &graph, It begin, It end, Visitor visitor);

template <class Graph, class Visitor>
void DepthFirstSearch(const Graph &graph, Visitor visitor);

namespace detail {

template <class Graph, class Visitor>
class Dfs {
 public:
  explicit Dfs(const Graph &graph, Visitor visitor)
      : graph_{graph},
        statuses_(NumberVertices(graph)),
        visitor_{std::move(visitor)} {}

  void Visit(int start) {
    std::stack<int> nodes;
    nodes.push(start);

    while (!nodes.empty()) {
      auto node = nodes.top();
      if (statuses_[node] == Status::LOOKING) {
        nodes.pop();
        statuses_[node] = Status::LOOKED;
        visitor_.FinishVertex(node);
        continue;
      }

      if (statuses_[node] == Status::LOOKED) {
        nodes.pop();
        continue;
      }

      statuses_[node] = Status::LOOKING;
      visitor_.DiscoverVertex(node);

      for (auto link : OutgoingEdges(graph_, node)) {
        auto target = GetTarget(graph_, link);
        if (statuses_[target] == Status::NEW) {
          nodes.push(target);
        }
      }
    }
  }

  template <class It>
  void PerformTraverse(It begin, It end) {
    for (auto it = begin; it != end; ++it) {
      auto node = *it;
      if (statuses_[node] == Status::NEW) {
        visitor_.StartVertex(node);
        Visit(node);
      }
    }
  }

 private:
  enum class Status {
    NEW,
    LOOKING,
    LOOKED,
  };

  const Graph &graph_;
  std::vector<Status> statuses_;
  Visitor visitor_;
};

}  // namespace detail

template <class Graph, class Visitor, class NodeType>
void DepthFirstSearch(const Graph &graph, Visitor visitor, NodeType start) {
  detail::Dfs<Graph, Visitor>{graph, std::move(visitor)}
      .Visit(start);
}

template <class Graph, class Visitor, class It>
void DepthFirstSearch(const Graph &graph, It begin, It end, Visitor visitor) {
  detail::Dfs<Graph, Visitor>{graph, std::move(visitor)}
      .PerformTraverse(begin, end);
}

template <class Graph, class Visitor>
void DepthFirstSearch(const Graph &graph, Visitor visitor) {
  std::vector<int> standard_order(graph.size());
  std::iota(standard_order.begin(), standard_order.end(), 0);
  return DepthFirstSearch(graph,
                          standard_order.begin(),
                          standard_order.end(),
                          visitor);
}

}  // namespace traverses
