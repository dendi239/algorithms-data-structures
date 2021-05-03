#pragma once

#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "graphs/vertex-property-map.hpp"
#include "graphs/edge-property-map.hpp"

namespace flows {

template <class Type, class BidirectionalGraph>
class PreFlow {
 public:
  using ValueType = Type;
  using VertexType = typename graphs::GraphTraits<BidirectionalGraph>::VertexType;
  using EdgeType = typename graphs::GraphTraits<BidirectionalGraph>::EdgeType;

  class EdgeDescriptor {
   public:
    operator Type() const {  // NOLINT
      return Get(*preflow_, edge_);
    }

    EdgeDescriptor &operator+=(Type diff) {
      At(preflow_->divergences_, from_) += diff;
      At(preflow_->divergences_, to_) -= diff;

      At(preflow_->edge_values_, edge_) += diff;
      At(preflow_->edge_values_, reversed_) -= diff;

      return *this;
    }

    EdgeDescriptor &operator-=(Type diff) {
      return *this += -diff;
    }

   private:
    friend class PreFlow;

    EdgeDescriptor(
      PreFlow *preflow, VertexType from, VertexType to,
      EdgeType edge, EdgeType reversed)
      : preflow_(preflow), from_(from), to_(to),
        edge_(edge), reversed_(reversed) {}

    PreFlow *preflow_;
    VertexType from_, to_;
    EdgeType edge_, reversed_;
  };

  explicit PreFlow(const BidirectionalGraph &bidirectional_graph)
    : bidirectional_graph_(bidirectional_graph),
      divergences_(bidirectional_graph),
      edge_values_(bidirectional_graph) {}

  EdgeDescriptor operator()(EdgeType edge) {
    EdgeType reversed = GetReversed(bidirectional_graph_, edge);
    VertexType from = GetTarget(bidirectional_graph_, reversed);
    VertexType to = GetTarget(bidirectional_graph_, edge);
    return EdgeDescriptor{this, from, to, edge, reversed};
  }

  ValueType operator()(EdgeType edge) const {
    return Get(edge_values_, edge);
  }

  ValueType Divergence(VertexType vertex) const {
    return Get(divergences_, vertex);
  }

 private:
  const BidirectionalGraph &bidirectional_graph_;
  graphs::VertexPropertyMap<BidirectionalGraph, Type> divergences_;
  graphs::EdgePropertyMap<BidirectionalGraph, Type> edge_values_;
};

}  // namespace flows
