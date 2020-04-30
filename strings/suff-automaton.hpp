#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <vector>

template <class Key, class Value>
struct FastCheapMap {
  bool count(Key key) const {
    auto it = std::lower_bound(data_.begin(), data_.end(), std::pair{key, Value{}});
    return it != data_.end() && it->first == key;
  }

  Value operator[](Key key) const {
    auto it = std::lower_bound(data_.begin(), data_.end(), std::pair{key, Value{}});
    if (it == data_.end()) {
      return Value{};
    } else {
      return it->second;
    }
  }

  Value &operator[](Key key) {
    auto it = std::lower_bound(data_.begin(), data_.end(), std::pair{key, Value{}});
    if (it == data_.end() || it->first != key) {
      data_.emplace_back(key, Value{});
      std::sort(data_.begin(), data_.end());
      return (*this)[key];
    }
    return it->second;
  }

  auto begin() const { return data_.begin(); }
  auto end() const { return data_.end(); }

 private:
  std::vector<std::pair<Key, Value>> data_;
};

class SuffAutomaton {
 public:
  explicit SuffAutomaton(std::string_view str = "") : nodes(1), last(0) {
    nodes.reserve(2 * str.size());
    for (auto ch : str) { Append(ch); }
  }

  size_t Size() const { return nodes.size(); }
  void Print() const;

  void Append(char ch);
  bool Contains(std::string_view str) const;

 private:
  struct Node {
    int len = 0, link = -1;
    FastCheapMap<char, int> next;
  };

  friend struct SuffAutomatonIntersector;
  friend struct MaxStringFinder;

  friend int NumberVertices(const SuffAutomaton &automaton);
  friend int GetTarget(const SuffAutomaton &automaton, std::pair<char, int> edge);
  friend const auto &OutgoingEdges(const SuffAutomaton &automaton, int node);

  std::vector<Node> nodes;
  int last;
};

int NumberVertices(const SuffAutomaton &automaton) {
  return automaton.nodes.size();
}

int GetTarget(const SuffAutomaton &automaton, std::pair<char, int> edge) {
  return edge.second;
}

const auto &OutgoingEdges(const SuffAutomaton &automaton, int node) {
  return automaton.nodes[node].next;
}

void SuffAutomaton::Print() const {
  for (size_t node = 0; node < nodes.size(); ++node) {
    std::cerr << "len " << node << " = " << nodes[node].len << '\n';
    for (auto [symb, next] : nodes[node].next) {
      std::cerr << node << " " << symb << " " << next << "\n";
    }
  }
  std::cerr << std::endl;
}

void SuffAutomaton::Append(char ch) {
  int new_id = static_cast<int>(nodes.size());
  nodes.push_back(Node());
  nodes.back().len = nodes[last].len + 1;

  int node;
  for (node = last; node != -1; node = nodes[node].link) {
    if (nodes[node].next.count(ch)) {
      break;
    } else {
      nodes[node].next[ch] = new_id;
    }
  }

  if (node == -1) {
    nodes[new_id].link = 0;
    last = new_id;
    return;
  }

  auto possible = nodes[node].next[ch];
  if (nodes[possible].len == nodes[node].len + 1) {
    nodes[new_id].link = possible;
    last = new_id;
    return;
  }

  auto actual = static_cast<int>(nodes.size());
  nodes.push_back(nodes[possible]);
  nodes.back().len = nodes[node].len + 1;

  for (auto it = node; it != -1 && nodes[it].next[ch] == possible; it = nodes[it].link) {
    nodes[it].next[ch] = actual;
  }

  nodes[possible].link = nodes[new_id].link = actual;
  last = new_id;
}

bool SuffAutomaton::Contains(std::string_view str) const {
  int node = 0;
  for (auto ch : str) {
    if (nodes[node].next.count(ch)) {
      node = nodes[node].next[ch];
    } else {
      return false;
    }
  }
  return true;
}
