#pragma once

#include <algorithm>
#include <array>
#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

class Ukkonen {
 public:
  explicit Ukkonen(std::string_view str = "")
      : tree_{Node{0, 0, -1}}, position_{0, 0, 0} {
    for (auto ch : str) {
      Append(ch);
    }
  }

  struct Position {
    int from, to;
    int shift;
  };

  bool Contains(std::string_view string_view) const;
  void Append(char ch);

  static Position Root() { return Position{0, 0, 0}; }

  using Link = std::pair<char, Position>;

 private:
  static constexpr int inf = 1 << 30;

  struct Node {
    int start = 0, shift = inf;
    int suff_link = 0;
    std::map<char, int> next;
  };

  friend struct UkkonenIntersector;
  friend auto GetTarget(const Ukkonen &, Link) -> Position;
  friend auto OutgoingEdges(const Ukkonen &, Position) -> std::vector<Link>;

  bool has(Position pos, char ch) const;
  Position go(Position pos, char ch) const;

  Position suff_link(Position pos) const;
  auto split_create(Position pos, char ch) -> std::pair<int, int>;
  int Split(Position pos);


#ifdef LOG_TREE

  void Print(int node, std::string pref = "", std::string step = "\t");

#endif // LOG_TREE

  std::string text_;
  int text_size_ = 0;
  std::vector<Node> tree_;
  Position position_;
};

bool operator==(Ukkonen::Position lhs, Ukkonen::Position rhs);

#ifdef LOG_TREE

#include <iomanip>

void Ukkonen::Print(int node, std::string pref, std::string step) {
  std::cerr << pref << std::setw(2) << node << " "
            << text_.substr(tree_[node].start, tree_[node].shift)
            << " $"[tree_[node].shift == inf]
            << " (" << tree_[node].suff_link << ")\n";
  if (node == position_.to) {
    std::cerr << pref << "   " << std::string(position_.shift, ' ') << "x\n";
  }
  for (auto [_, child] : tree_[node].next) {
    Print(child, pref + std::string(tree_[node].shift, ' '));
  }
}

#endif  // LOG_TREE

bool Ukkonen::Contains(std::string_view string_view) const {
  Position pos = Root();
  for (auto ch : string_view) {
    if (!has(pos, ch)) return false;
    pos = go(pos, ch);
  }
  return true;
}

auto Ukkonen::suff_link(Ukkonen::Position pos) const -> Position {
  int node = 0, next_node = 0, shift = pos.shift, location = tree_[pos.to].start;
  if (pos.from == 0) {
    location += 1, shift -= 1;
  } else {
    node = tree_[pos.from].suff_link;
  }
  if (shift > 0) next_node = tree_[node].next.at(text_[location]);

  while (tree_[next_node].shift < shift) {
    const int step = tree_[next_node].shift;
    location += step;
    shift -= step;
    node = next_node;
    next_node = tree_[node].next.at(text_[location]);
  }

  return Position{node, next_node, shift};
}

int Ukkonen::Split(Position pos) {
  int new_id = static_cast<int>(tree_.size());
  const int start = tree_[pos.to].start;
  const char current = text_[start + pos.shift];

  Node new_node{tree_[pos.to].start, pos.shift, 0};
  new_node.next[current] = pos.to;
  tree_.push_back(new_node);

  tree_[pos.from].next[text_[start]] = new_id;
  tree_[pos.to].start += pos.shift;
  if (tree_[pos.to].shift != inf) tree_[pos.to].shift -= pos.shift;

  return new_id;
}

std::pair<int, int> Ukkonen::split_create(Position pos, char ch) {
  auto add_link = [&](int node) {
    int new_id = static_cast<int>(tree_.size());
    Node new_node{static_cast<int>(text_.size()) - 1, inf, 0, {}};
    tree_[node].next[ch] = new_id;
    tree_.push_back(new_node);
    return new_id;
  };

  int id;
  if (pos.shift < tree_[pos.to].shift) {
    id = Split(pos);
  } else {
    id = pos.to;
  }

  return {id, add_link(id)};
}

auto Ukkonen::go(Position pos, char ch) const -> Position {
  if (pos.shift < tree_[pos.to].shift) {
    pos.shift += 1;
    return pos;
  } else {
    return Position{pos.to, tree_[pos.to].next.at(ch), 1};
  }
}

bool Ukkonen::has(Ukkonen::Position pos, char ch) const {
  if (pos.shift < tree_[pos.to].shift) {
    int start = tree_[pos.to].start;
    return text_[start + pos.shift] == ch;
  } else {
    return tree_[pos.to].next.count(ch);
  }
}

void Ukkonen::Append(char ch) {
  text_.push_back(ch), ++text_size_;

  std::pair<int, int> last_id{-1, -1};
  while (!has(position_, ch)) {
    auto new_id = split_create(position_, ch);
    position_.to = new_id.first;
    if (last_id.first != -1) {
      tree_[last_id.first].suff_link = new_id.first;
      tree_[last_id.second].suff_link = new_id.second;
    }
    last_id = new_id;
    position_ = position_.shift == 0 ? position_ : suff_link(position_);
  }

  position_ = go(position_, ch);
  if (last_id.first != -1) {
    tree_[last_id.first].suff_link = position_.from;
    tree_[last_id.second].suff_link = position_.to;
  }

  if (position_.shift + tree_[position_.to].start == text_size_) {
    position_ = suff_link(position_);
  }

#ifdef LOG_TREE
  std::cerr << "-> " << text_ << "\n";
  Print(0);
  std::cerr << std::endl;
#endif  // LOG_TREE

#ifdef DEBUG
  for (size_t i = 0; i < text_.size(); ++i) {
    assert(Contains(text_.substr(i)));
  }
#endif // DEBUG
}

bool operator==(Ukkonen::Position lhs, Ukkonen::Position rhs) {
  return lhs.to == rhs.to
      && lhs.shift == rhs.shift;
}

namespace std {

template <>
struct hash<Ukkonen::Position> {
  size_t operator()(Ukkonen::Position position) const noexcept {
    return (static_cast<size_t>(position.to) << 30) ^
           (static_cast<size_t>(position.shift));
  }
};

}  // namespace std

Ukkonen::Position GetTarget(
    const Ukkonen &/*ukkonen*/,
    std::pair<char, Ukkonen::Position> edge) {
  return edge.second;
}

std::vector<Ukkonen::Link> OutgoingEdges(
    const Ukkonen &ukkonen,
    Ukkonen::Position position) {
  if (ukkonen.tree_[position.to].shift != position.shift) {
    auto symb = ukkonen.text_[ukkonen.tree_[position.to].start + position.shift];
    auto next = Ukkonen::Position{position.from, position.to, position.shift + 1};
    return {{symb, next}};
  } else {
    std::vector<Ukkonen::Link> links;
    auto &storage = ukkonen.tree_[position.to].next;
    for (auto [symb, next_node] : storage) {
      if (!storage.count(symb)) continue;
      links.emplace_back(symb, Ukkonen::Position{position.to, next_node, 1});
    }
    return links;
  }
}
