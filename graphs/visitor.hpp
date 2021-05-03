#pragma once

#include <functional>

#include "graphs/graph.hpp"

namespace traverses {

namespace detail {

struct DoNothing {
  template <class T>
  void operator()(T) const {
    // no-op
  }
};

}  // namespace detail

template <
  class Vertex = int,
  class StartFunction = detail::DoNothing,
  class DiscoverFunction = detail::DoNothing,
  class FinishFunction = detail::DoNothing>
class Visitor {
 public:
  Visitor(StartFunction on_start = {},
          DiscoverFunction on_discover = {},
          FinishFunction on_finish = {})
    : on_start_{on_start}, on_discover_{on_discover}, on_finish_{on_finish} {}

  void StartVertex(Vertex vertex) const { on_start_(vertex); }
  void DiscoverVertex(Vertex vertex) const { on_discover_(vertex); }
  void FinishVertex(Vertex vertex) const { on_finish_(vertex); }

  template <class NewStartFunction>
  auto OnStart(NewStartFunction new_start_function) const {
    return Visitor<Vertex, NewStartFunction, DiscoverFunction, FinishFunction>{
      new_start_function, on_discover_, on_finish_
    };
  }

  template <class NewDiscoverFunction>
  auto OnDiscover(NewDiscoverFunction new_discover_function) const {
    return Visitor<Vertex, StartFunction, NewDiscoverFunction, FinishFunction>{
      on_start_, new_discover_function, on_finish_
    };
  }

  template <class NewFinishFunction>
  auto OnFinish(NewFinishFunction new_finish_function) const {
    return Visitor<Vertex, StartFunction, DiscoverFunction, NewFinishFunction>{
      on_start_, on_discover_, new_finish_function
    };
  }

 private:
  StartFunction on_start_;
  DiscoverFunction on_discover_;
  FinishFunction on_finish_;
};

template <class Graph>
using VisitorBuilder = Visitor<typename GraphTraits<Graph>::VertexType>;

}  // namespace traverses
