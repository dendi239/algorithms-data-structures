#include <cstdint>
#include <numeric>
#include <vector>

using Node = size_t;

template <class Iterator>
class IteratorRange {
    Iterator begin_, end_;

public:
    IteratorRange(Iterator begin, Iterator end) : begin_{std::move(begin)}, end_{std::move(end)} {
        // no-op
    }

    template <class Container>
    explicit IteratorRange(const Container &container)
    : begin_{std::begin(container)}, end_(std::end(container)) {
        // no-op
    }

    auto begin() const {
        return begin_;
    }

    auto end() const {
        return end_;
    }
};

class Graph {
    std::vector<std::vector<Node>> adjacency_list_;

public:
    explicit Graph(size_t size) : adjacency_list_(size) {
    }

    size_t Size() const {
        return adjacency_list_.size();
    }

    void Connect(Node first, Node second);
    auto Links(Node node) const -> IteratorRange<std::vector<Node>::const_iterator>;
};

class Tree : public Graph {
    Node root_;

public:
    explicit Tree(size_t size) : Graph(size), root_{0} {
    }

    inline auto Root() const -> Node {
        return root_;
    }

    inline void SetRoot(Node root) {
        root_ = root;
    }
};

template <class Concrete>
struct DfsVisitor {
    void StartVertex(Node node);
    void DiscoverVertex(Node node);
    void FinishVertex(Node node);

    template <class GraphType = Graph>
    void Visit(const GraphType &graph);
    template <class GraphType = Graph, class It>
    void Visit(const GraphType &graph, It begin, It end);
    template <class GraphType = Graph, class Container = std::vector<Node>>
    void Visit(const GraphType &graph, Container order);
};

template <class StartFunc, class DiscoverFunc, class FinishFunc>
class LambdaVisitor : public DfsVisitor<LambdaVisitor<StartFunc, DiscoverFunc, FinishFunc>> {
    StartFunc start_func_;
    DiscoverFunc discover_func_;
    FinishFunc finish_func_;

public:
    LambdaVisitor(StartFunc start_func = StartFunc(),
                  DiscoverFunc discover_func = DiscoverFunc(),
                  FinishFunc finish_func = FinishFunc())
        : start_func_{std::move(start_func)},
          discover_func_{std::move(discover_func)},
          finish_func_{std::move(finish_func)} {
    }

    template <class NewStartFunc>
    auto OnStart(NewStartFunc new_start_func) {
        using ReturnType = LambdaVisitor<NewStartFunc, DiscoverFunc, FinishFunc>;
        return ReturnType(new_start_func, discover_func_, finish_func_);
    }

    template <class NewDiscoverFunc>
    auto OnDiscover(NewDiscoverFunc new_discover_func) {
        using ReturnType = LambdaVisitor<StartFunc, NewDiscoverFunc, FinishFunc>;
        return ReturnType(start_func_, new_discover_func, finish_func_);
    }

    template <class NewFinishFunc>
    auto OnFinish(NewFinishFunc new_finish_func) {
        using ReturnType = LambdaVisitor<StartFunc, DiscoverFunc, NewFinishFunc>;
        return ReturnType(start_func_, discover_func_, new_finish_func);
    }

    void StartVertex(Node node) {
        return start_func_(node);
    }

    void DiscoverVertex(Node node) {
        return discover_func_(node);
    }

    void FinishVertex(Node node) {
        return finish_func_(node);
    }
};

template <class ...Args>
struct DoNothing {
    void operator()(Args ...) const noexcept {
        // no-op
    }
};

auto DfsVisitorBuilder() {
    return LambdaVisitor(DoNothing<Node>(), DoNothing<Node>(), DoNothing<Node>());
}

template <class GraphType, class Visitor>
class Dfs {
    enum class Status {
        NEW,
        LOOKING,
        LOOKED,
    };

    const GraphType &graph_;
    std::vector<Status> statuses_;
    Visitor visitor_;

public:
    explicit Dfs(const GraphType &graph, Visitor visitor)
        : graph_{graph}, statuses_(graph.Size()), visitor_{std::move(visitor)} {
    }

    void Visit(Node node) {
        visitor_.DiscoverVertex(node);
        statuses_[node] = Status::LOOKING;

        auto range = graph_.Links(node);
        for (auto link : range) {
            if (statuses_[link] == Status::NEW) {
                Visit(link);
            }
        }
        statuses_[node] = Status::LOOKED;
        visitor_.FinishVertex(node);
    }

    template <class It>
    void PerformTraverse(It begin, It end) {
        for (auto it = begin; it != end; ++it) {
            auto node = *it;
            if (statuses_[node] == Status::NEW) {
                visitor_.StartVertex(node);
                Visit(node);
            }
        }
    }
};

void Graph::Connect(Node first, Node second) {
    adjacency_list_[first].push_back(second);
    adjacency_list_[second].push_back(first);
}

auto Graph::Links(Node node) const -> IteratorRange<std::vector<Node>::const_iterator> {
    return IteratorRange<std::vector<Node>::const_iterator>(adjacency_list_[node]);
}

template <class GraphType, class Visitor, class It>
void TraverseGraphInDfsOrder(const GraphType &graph, It begin, It end, Visitor visitor) {
    Dfs<GraphType, Visitor>{graph, std::move(visitor)}
        .PerformTraverse(begin, end);
}

template <class GraphType, class Visitor>
void TraverseGraphInDfsOrder(const GraphType &graph, Visitor visitor) {
    std::vector<Node> standard_order(graph.Size());
    std::iota(standard_order.begin(), standard_order.end(), 0);
    return TraverseGraphInDfsOrder(graph, standard_order.begin(), standard_order.end(), visitor);
}

template <class Concrete>
template <class GraphType>
void DfsVisitor<Concrete>::Visit(const GraphType &graph) {
    return TraverseGraphInDfsOrder(graph, *static_cast<Concrete *>(this));
}

template <class Concrete>
template <class GraphType, class It>
void DfsVisitor<Concrete>::Visit(const GraphType &graph, It begin, It end) {
    return TraverseGraphInDfsOrder(graph, begin, end, *static_cast<Concrete *>(this));
}

template <class Concrete>
template <class GraphType, class Container>
void DfsVisitor<Concrete>::Visit(const GraphType &graph, Container order) {
    return Visit(graph, begin(order), end(order));
}

template <class Concrete>
void DfsVisitor<Concrete>::StartVertex(Node node) {
    static_cast<Concrete *>(this)->StartVertex(node);
}

template <class Concrete>
void DfsVisitor<Concrete>::DiscoverVertex(Node node) {
    static_cast<Concrete *>(this)->DiscoverVertex(node);
}

template <class Concrete>
void DfsVisitor<Concrete>::FinishVertex(Node node) {
    static_cast<Concrete *>(this)->FinishVertex(node);
}
