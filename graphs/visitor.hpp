#pragma once

#include <functional>

class LambdaVisitor {
 public:
  LambdaVisitor() = default;
  LambdaVisitor(std::function<void(int)> on_start,
                std::function<void(int)> on_discover,
                std::function<void(int)> on_finish);

  void StartVertex(int node) const;
  void DiscoverVertex(int node) const;
  void FinishVertex(int node) const;

  template <class StartFunction>
  LambdaVisitor OnStart(StartFunction start_function);
  template <class DiscoverFunction>
  LambdaVisitor OnDiscover(DiscoverFunction discover_function);
  template <class FinishFunction>
  LambdaVisitor OnFinish(FinishFunction finish_function);

  template <class Graph>
  void Visit(const Graph &graph) const;
  template <class Graph, class Order>
  void Visit(const Graph &graph, const Order &order) const;
  template <class Graph, class It>
  void Visit(const Graph &graph, It begin, It end) const;

 private:
  std::function<void(int)> on_start_;
  std::function<void(int)> on_discover_;
  std::function<void(int)> on_finish_;
};


template <class Graph, class It>
void LambdaVisitor::Visit(const Graph &graph, It begin, It end) const {
  TraverseGraphInDfsOrder(graph, begin, end, *this);
}

template <class Graph, class Order>
void LambdaVisitor::Visit(const Graph &graph, const Order &order) const {
  TraverseGraphInDfsOrder(graph, order.begin(), order.end(), *this);
}

template <class StartFunction>
LambdaVisitor LambdaVisitor::OnStart(StartFunction start_function) {
  return LambdaVisitor{
      std::function<void(int)>(start_function),
      on_discover_,
      on_finish_,
  };
}

template <class DiscoverFunction>
LambdaVisitor LambdaVisitor::OnDiscover(DiscoverFunction discover_function) {
  return LambdaVisitor{
      on_start_,
      std::function<void(int)>(discover_function),
      on_finish_,
  };
}

template <class FinishFunction>
LambdaVisitor LambdaVisitor::OnFinish(FinishFunction finish_function) {
  return LambdaVisitor{
      on_start_,
      on_discover_,
      std::function<void(int)>(finish_function),
  };
}

void LambdaVisitor::StartVertex(int node) const {
  if (on_start_) {
    on_start_(node);
  }
}

void LambdaVisitor::DiscoverVertex(int node) const {
  if (on_discover_) {
    on_discover_(node);
  }
}

void LambdaVisitor::FinishVertex(int node) const {
  if (on_finish_) {
    on_finish_(node);
  }
}

LambdaVisitor::LambdaVisitor(std::function<void(int)> on_start,
                             std::function<void(int)> on_discover,
                             std::function<void(int)> on_finish)
    : on_start_{on_start}, on_discover_{on_discover}, on_finish_{on_finish} {
  // no-op
}
