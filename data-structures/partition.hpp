#pragma once

#include <vector>

class Partition {
 public:
  size_t GroupsNumber() const { return components_.size(); }
  size_t GroupId(int node) const { return component_by_node_[node]; }

  auto Elements(size_t group_id) const -> const std::vector<int> & {
    return components_[group_id];
  }

 private:
  friend class PartitionBuilder;

  explicit Partition(std::vector<std::vector<int>> components, std::vector<size_t> component_by_node)
      : components_{std::move(components)}, component_by_node_{std::move(component_by_node)}
  {}

  std::vector<std::vector<int>> components_;
  std::vector<size_t> component_by_node_;
};

class PartitionBuilder {
 public:
  explicit PartitionBuilder(size_t graph_size)
      : component_by_node_(graph_size)
  {}

  size_t CreateNewGroup() {
    components_.emplace_back();
    return components_.size() - 1;
  }

  void AddElementToGroup(int node, size_t index) {
    component_by_node_[node] = index;
    components_[index].push_back(node);
  }

  void AddElementToLastGroup(int node) {
    AddElementToGroup(node, components_.size() - 1);
  }

  Partition Build() {
    return Partition{
      std::move(components_),
      std::move(component_by_node_),
    };
  }

 private:
  std::vector<std::vector<int>> components_;
  std::vector<size_t> component_by_node_;
};
