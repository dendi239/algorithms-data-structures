#pragma once

#include <array>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include <ranges/iterator_range.hpp>
#include <graphs/bfs.hpp>

namespace aho_corasick {

class NodeReference;

struct Node {
  static const int kBadId = -1;

  int suff_link = kBadId, filled_suff_link = kBadId;
  std::vector<size_t> ids;

  std::map<char, int> children;

  auto At(char ch) const -> int;
};

struct BaseGraph {
  static const int kRootId = 0;

  BaseGraph() : nodes_(1) {}
  explicit BaseGraph(std::vector<Node> nodes) : nodes_{std::move(nodes)} {}

  std::vector<Node> nodes_;

  auto Link(int node, char ch) const -> int
  { return nodes_[node].At(ch); }

  bool Contains(int node, char ch) const
  { return nodes_[node].At(ch) != Node::kBadId; }

  auto SuffLink(int node) const -> int
  { return nodes_[node].suff_link; }

  auto FilledSuffLink(int node) const -> int
  { return nodes_[node].filled_suff_link; }

  auto Ids(int node) const -> const std::vector<size_t> &
  { return nodes_[node].ids; }
};

class Automaton : private BaseGraph {
 public:
  NodeReference Root() const;

 private:
  friend class NodeReference;
  friend class AutomatonBuilder;

  explicit Automaton(std::vector<Node> nodes) : BaseGraph(std::move(nodes)) {}
};

class AutomatonBuilder : private BaseGraph {
 public:
  void Add(std::string_view pattern, size_t identifier);
  auto Build() -> std::unique_ptr<Automaton>;

 private:
  friend class AutomatonVisitor;

  friend IteratorRange<std::map<char, int>::const_iterator> OutgoingEdges(
      const AutomatonBuilder &, int);
  friend int GetTarget(const AutomatonBuilder &, const std::pair<char, int> &);

  int GetOrCreateLink(int node, char ch);
};

class NodeReference {
 public:
  auto Avaliable() const -> std::vector<char>;
  auto Next(char symbol) const -> NodeReference;

  bool operator==(const NodeReference &rhs) const;

  template <class Callback>
  void GenerateMatches(Callback callback) const;

 private:
  friend class Automaton;
  friend class std::hash<NodeReference>;

  NodeReference(int node, const Automaton &automaton)
      : node_{node}, automaton_{&automaton}
  {}

  int node_;
  const Automaton *automaton_;
};

}  // namespace aho_corasick

namespace std {

template <>
struct hash<aho_corasick::NodeReference> {
  size_t operator()(const aho_corasick::NodeReference &reference) const noexcept {
    return reference.node_;
  }
};

}  // namespace std

namespace aho_corasick {

using NodeType = int;
using EdgeType = std::pair<char, int>;

int GetTarget(const AutomatonBuilder &/*graph*/, const EdgeType &edge) {
  return edge.second;
}

IteratorRange<std::map<char, int>::const_iterator> OutgoingEdges(
    const AutomatonBuilder &graph, int node) {
  auto &links = graph.nodes_[node].children;
  return IteratorRange(links.begin(), links.end());
}

class AutomatonVisitor : public traverses::BfsVisitor<NodeType, EdgeType> {
 public:
  AutomatonVisitor(AutomatonBuilder *graph, int node)
      : graph_{graph}, node_{node}
  {}

  virtual void ExamineVertex(int vertex) override {
    node_ = vertex;
  }

  virtual void ExamineEdge(const EdgeType &edge) override {
    auto [ch, link] = edge;

    auto &suff_link = graph_->nodes_[link].suff_link;
    auto &filled_suff_link = graph_->nodes_[link].filled_suff_link;

    for (auto node = graph_->SuffLink(node_); node != Node::kBadId;
         node = graph_->SuffLink(node)) {
      if (!graph_->Contains(node, ch)) continue;
      suff_link = graph_->Link(node, ch);
      break;
    }

    if (suff_link == Node::kBadId) suff_link = AutomatonBuilder::kRootId;

    filled_suff_link = graph_->Ids(suff_link).empty()
                       ? graph_->nodes_[suff_link].filled_suff_link
                       : suff_link;
  }

 private:
  AutomatonBuilder *graph_;
  int node_ = AutomatonBuilder::kRootId;
};

std::unique_ptr<Automaton> AutomatonBuilder::Build() {
  auto visitor = AutomatonVisitor{this, kRootId};
  traverses::BreadthFirstSearch(kRootId, *this, visitor);

  auto nodes = std::move(nodes_);
  nodes_ = {Node()};

  return std::unique_ptr<Automaton>(new Automaton(std::move(nodes)));
}

NodeReference Automaton::Root() const {
  return NodeReference(kRootId, *this);
}

int AutomatonBuilder::GetOrCreateLink(int node, char ch) {
  if (nodes_[node].At(ch) == Node::kBadId) {
    nodes_.push_back(Node());
    int last_id = static_cast<int>(nodes_.size()) - 1;
    nodes_[node].children[ch] = last_id;
  }

  return nodes_[node].At(ch);
}

void AutomatonBuilder::Add(std::string_view pattern, size_t identifier) {
  int node = kRootId;
  for (auto ch : pattern) {
    node = GetOrCreateLink(node, ch);
  }
  nodes_[node].ids.push_back(identifier);
}

auto Node::At(char ch) const -> int {
  if (children.count(ch)) {
    return children.at(ch);
  } else {
    return kBadId;
  }
}

bool NodeReference::operator==(const NodeReference &rhs) const {
  return std::tie(node_, automaton_) == std::tie(rhs.node_, rhs.automaton_);
}

template <class Callback>
void NodeReference::GenerateMatches(Callback callback) const {
  for (int node = node_; node != Node::kBadId;
       node = automaton_->FilledSuffLink(node)) {
    for (auto id : automaton_->Ids(node)) callback(id);
  }
}

auto NodeReference::Next(char symbol) const -> NodeReference {
  int node = node_;

  while (!automaton_->Contains(node, symbol) && node != Automaton::kRootId) {
    node = automaton_->SuffLink(node);
  }
  if (automaton_->Contains(node, symbol)) {
    node = automaton_->Link(node, symbol);
  }

  return NodeReference(node, *automaton_);
}

}  // namespace aho_corasick
