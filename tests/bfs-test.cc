#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include "../ranges/iterator_range.hpp"
#include "../graphs/bfs.hpp"

void TestBfsHalt();
void TestBfsOrder();

int main() {
  TestBfsHalt();      std::cerr << "BfsHalt tests are passed!\n";
  TestBfsOrder();     std::cerr << "BfsOrder tests are passed!\n";
}

template<class T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vector) {
  std::copy(vector.begin(), vector.end(), std::ostream_iterator<T>(os, " "));
  return os;
}

class TestNotPassedException : public std::runtime_error {
 public:
  explicit TestNotPassedException(const char *what)
      : std::runtime_error(what) {}

  explicit TestNotPassedException(const std::string &what)
      : std::runtime_error(what.c_str()) {}
};


#define REQUIRE_EQUAL(first, second)                                               \
    do {                                                                           \
        auto firstValue = (first);                                                 \
        auto secondValue = (second);                                               \
        if (!(firstValue == secondValue)) {                                        \
            std::ostringstream oss;                                                \
            oss << "Require equal failed: " << #first << " != " << #second << " (" \
                    << firstValue << " != " << secondValue << ")\n";               \
            throw TestNotPassedException(oss.str());                               \
        }                                                                          \
    } while (false)

namespace test_bfs {

typedef std::vector<int> VerticesList;

struct Graph {
  std::vector<VerticesList> adjacent_vertices;
};

int GetTarget(const Graph & /*graph_*/, int edge) { return edge; }

IteratorRange<VerticesList::const_iterator> OutgoingEdges(const Graph &graph,
                                                          int vertex) {
  return IteratorRange<VerticesList::const_iterator>(
      graph.adjacent_vertices[vertex].begin(),
      graph.adjacent_vertices[vertex].end());
}

enum class BfsEvent {
  kDiscoverVertex, kExamineEdge, kExamineVertex
};

std::ostream& operator<<(std::ostream& out, const BfsEvent& event) {
  switch (event) {
    case BfsEvent::kDiscoverVertex:
      out << "discover";
      break;
    case BfsEvent::kExamineEdge:
      out << "examine edge";
      break;
    case BfsEvent::kExamineVertex:
      out << "examine vertex";
      break;
  }
  return out;
}

std::ostream& operator<<(std::ostream& os, const std::pair<BfsEvent, int>& item) {
  os << "(" << item.first << ", " << item.second << ")";
  return os;
}

class TestBfsVisitor: public traverses::BfsVisitor<int, int> {
 public:
  explicit TestBfsVisitor(std::vector<std::pair<BfsEvent, int>> *events) {
    events_ = events;
  }

  virtual void DiscoverVertex(int vertex) override {
    events_->emplace_back(BfsEvent::kDiscoverVertex, vertex);
  }
  virtual void ExamineEdge(const int& edge) override {
    events_->emplace_back(BfsEvent::kExamineEdge, edge);
  }
  virtual void ExamineVertex(int vertex) override {
    events_->emplace_back(BfsEvent::kExamineVertex, vertex);
  }

 private:
  std::vector<std::pair<BfsEvent, int>> *events_;
};
}  // namespace test_bfs

void TestBfsHalt() {
  test_bfs::Graph graph;
  graph.adjacent_vertices.resize(4);
  const int a = 0;
  const int b = 1;
  const int c = 2;
  const int d = 3;
  graph.adjacent_vertices[a].push_back(b);
  graph.adjacent_vertices[b].push_back(c);
  graph.adjacent_vertices[c].push_back(d);
  graph.adjacent_vertices[d].push_back(b);
  traverses::BreadthFirstSearch(a, graph, traverses::BfsVisitor<int, int>());
  // BreadthFirstSearch should not hang on A. Corruption graph_ with A. Corruption cycle
}

void TestBfsOrder() {
  using namespace test_bfs;
  Graph graph;
  graph.adjacent_vertices.resize(4);
  const int a = 0;
  const int b = 1;
  const int c = 2;
  const int d = 3;
  graph.adjacent_vertices[a].push_back(b);
  graph.adjacent_vertices[a].push_back(c);
  graph.adjacent_vertices[b].push_back(d);
  graph.adjacent_vertices[c].push_back(d);
  graph.adjacent_vertices[d].push_back(a);

  std::vector<std::pair<BfsEvent, int>> events;
  TestBfsVisitor visitor(&events);
  traverses::BreadthFirstSearch(a, graph, visitor);

  std::vector<std::pair<BfsEvent, int>> expected{
      {BfsEvent::kDiscoverVertex, a},
      {BfsEvent::kExamineVertex, a},
      {BfsEvent::kExamineEdge, b},
      {BfsEvent::kDiscoverVertex, b},
      {BfsEvent::kExamineEdge, c},
      {BfsEvent::kDiscoverVertex, c},
      {BfsEvent::kExamineVertex, b},
      {BfsEvent::kExamineEdge, d},
      {BfsEvent::kDiscoverVertex, d},
      {BfsEvent::kExamineVertex, c},
      {BfsEvent::kExamineEdge, d},
      {BfsEvent::kExamineVertex, d},
      {BfsEvent::kExamineEdge, a}
  };

  REQUIRE_EQUAL(events, expected);
}
