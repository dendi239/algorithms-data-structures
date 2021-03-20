#pragma once

#include <array>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "../ranges/iterator_range.hpp"
#include "../graphs/bfs.hpp"

namespace aho_corasick {

class NodeReference;

struct AutomatonNode {
  AutomatonNode() : suffix_link(nullptr), terminal_link(nullptr) {}

  std::vector<size_t> terminated_string_ids;
  std::map<char, AutomatonNode> trie_transitions;

  std::map<char, AutomatonNode *> automaton_transitions_cache;
  AutomatonNode *suffix_link;
  AutomatonNode *terminal_link;
};

// Returns a corresponding trie transition 'nullptr' otherwise.
AutomatonNode *GetTrieTransition(AutomatonNode *node, char character) {
  if (auto it = node->trie_transitions.find(character);
      it != node->trie_transitions.end()) {
    return &it->second;
  }
  return nullptr;
}

AutomatonNode *GetAutomatonTransition(AutomatonNode *node,
                                      AutomatonNode *root,
                                      char character) {
  if (node == nullptr) {
    return root;
  }

  if (auto trie_transition = GetTrieTransition(node, character)) {
    return node->automaton_transitions_cache[character] = trie_transition;
  }

  if (auto it = node->automaton_transitions_cache.find(character);
      it != node->automaton_transitions_cache.end()) {
    return it->second;
  }

  AutomatonNode *transition = GetAutomatonTransition(
      node->suffix_link, root, character);
  return node->automaton_transitions_cache[character] = transition;
}

namespace internal {

class AutomatonGraph {
 public:
  struct Edge {
    Edge(AutomatonNode *source, AutomatonNode *target, char character)
        : source(source), target(target), character(character) {}

    AutomatonNode *source;
    AutomatonNode *target;
    char character;
  };
};

std::vector<typename AutomatonGraph::Edge> OutgoingEdges(
    const AutomatonGraph & /*graph*/, AutomatonNode *vertex) {
  std::vector<typename AutomatonGraph::Edge> edges;
  for (auto &[symbol, destination] : vertex->trie_transitions) {
    edges.emplace_back(vertex, &destination, symbol);
  }
  return edges;
}

AutomatonNode *GetTarget(const AutomatonGraph & /*graph*/,
                         const AutomatonGraph::Edge &edge) {
  return edge.target;
}

class SuffixLinkCalculator
    : public traverses::BfsVisitor<AutomatonNode *, AutomatonGraph::Edge> {
 public:
  explicit SuffixLinkCalculator(AutomatonNode *root) : root_(root) {}

  void ExamineEdge(const AutomatonGraph::Edge &edge) override {
    if (edge.source->suffix_link == nullptr) {
      edge.target->suffix_link = root_;
    } else {
      edge.target->suffix_link = GetAutomatonTransition(
          edge.source->suffix_link, root_, edge.character);
    }
  }

 private:
  AutomatonNode *root_;
};

class TerminalLinkCalculator
    : public traverses::BfsVisitor<AutomatonNode *, AutomatonGraph::Edge> {
 public:
  explicit TerminalLinkCalculator(AutomatonNode *root) : root_(root) {}

  void DiscoverVertex(AutomatonNode *node) override {
    if (node->suffix_link == nullptr) {
      return;
    }
    if (node->suffix_link->terminated_string_ids.empty()) {
      node->terminal_link = node->suffix_link->terminal_link;
    } else {
      node->terminal_link = node->suffix_link;
    }
  }

 private:
  AutomatonNode *root_;
};

} // namespace internal

class NodeReference {
 public:
  NodeReference() : node_(nullptr), root_(nullptr) {}

  NodeReference(AutomatonNode *node, AutomatonNode *root)
      : node_(node), root_(root) {}

  NodeReference Next(char character) const {
    return NodeReference{
      GetAutomatonTransition(node_, root_, character),
      root_,
    };
  }

  template <class Callback>
  void GenerateMatches(Callback on_match) const {
    for (NodeReference node = *this; node; node = node.TerminalLink()) {
      for (auto terminated_id : node.TerminatedStringIds()) {
        on_match(terminated_id);
      }
    }
  }

  bool IsTerminal() const {
    return !node_->terminated_string_ids.empty();
  }

  explicit operator bool() const { return node_ != nullptr; }

  bool operator==(NodeReference other) const {
    return node_ == other.node_ && root_ == other.root_;
  }

 private:
  using TerminatedStringIterator = std::vector<size_t>::const_iterator;
  using TerminatedStringIteratorRange = IteratorRange<TerminatedStringIterator>;

  NodeReference TerminalLink() const {
    return NodeReference{node_->terminal_link, root_};
  }

  TerminatedStringIteratorRange TerminatedStringIds() const {
    return IteratorRange(node_->terminated_string_ids);
  }

  AutomatonNode *node_;
  AutomatonNode *root_;
};

class AutomatonBuilder;

class Automaton {
 public:
  Automaton() = default;

  Automaton(const Automaton &) = delete;
  Automaton &operator=(const Automaton &) = delete;

  NodeReference Root() {
    return NodeReference(&root_, &root_);
  }

 private:
  AutomatonNode root_;

  friend class AutomatonBuilder;
};

class AutomatonBuilder {
 public:
  void Add(const std::string &string, size_t id) {
    words_.push_back(string);
    ids_.push_back(id);
  }

  std::unique_ptr<Automaton> Build() {
    auto automaton = std::make_unique<Automaton>();
    BuildTrie(words_, ids_, automaton.get());
    BuildSuffixLinks(automaton.get());
    BuildTerminalLinks(automaton.get());
    return automaton;
  }

 private:
  static void BuildTrie(const std::vector<std::string> &words,
                        const std::vector<size_t> &ids, Automaton *automaton) {
    for (size_t i = 0; i < words.size(); ++i) {
      AddString(&automaton->root_, ids[i], words[i]);
    }
  }

  static void AddString(AutomatonNode *root, size_t string_id,
                        const std::string &string) {
    AutomatonNode *current = root;
    for (char character : string) {
      // use implicit ctor if necessary
      current = &current->trie_transitions[character];
    }
    current->terminated_string_ids.push_back(string_id);
  }

  static void BuildSuffixLinks(Automaton *automaton) {
    traverses::BreadthFirstSearch(
        &automaton->root_, internal::AutomatonGraph{},
        internal::SuffixLinkCalculator(&automaton->root_));
  }

  static void BuildTerminalLinks(Automaton *automaton) {
    traverses::BreadthFirstSearch(
        &automaton->root_, internal::AutomatonGraph{},
        internal::TerminalLinkCalculator(&automaton->root_));
  }

  std::vector<std::string> words_;
  std::vector<size_t> ids_;
};

}  // namespace aho_corasick
