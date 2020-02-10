#include <cstdint>
#include <numeric>
#include <vector>

#include "../ranges/ranges.hpp"

class Graph {
 public:
  using Node = size_t;
  using Nodes = IteratorRange<std::vector<Node>::const_iterator>;

  explicit Graph(size_t size) : adjacency_list_(size) {}

  size_t Size() const { return adjacency_list_.size(); }

  void Connect(Node first, Node second);
  auto Links(Node node) const -> Nodes;

 private:
  std::vector<std::vector<Node>> adjacency_list_;
};

class Tree : public Graph {
 public:
  explicit Tree(size_t size, Node root = 0) : Graph(size), root_{root} {}

  inline auto Root() const -> Node { return root_; }

  inline void SetRoot(Node root) { root_ = root; }

 private:
  Node root_;
};

void Graph::Connect(Node first, Node second) {
  adjacency_list_[first].push_back(second);
  adjacency_list_[second].push_back(first);
}

auto Graph::Links(Node node) const -> Graph::Nodes {
  return Nodes(adjacency_list_[node]);
}
