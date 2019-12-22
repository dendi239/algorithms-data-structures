# Graph

## Representations

Currently, in initial commit only pretty raw interface with adjacency list is supported. Moreover, only one concrete graph representation is allowed for unidirect unweights graph.

TODO
- [ ] support graph_traits for any graph representation
- [ ] add weitheted graph data structure
- [ ] ensure present algorithms work fine for all of them

## Algorithms

Currently, there's DFS presented. It's planned for other algorithms to appear in near future. Along with examples.

TODO List for algorithms:
- [X] DFS
    - [ ] Finding strongly-connected-component
    - [ ] Topological sort
    - [ ] Finding bridges
    - [ ] Condensation
- [ ] BFS
- [ ] Dijkstra
- [ ] Ford-Belman 
- [ ] Complete list of algorithms
- [ ] Lca

### DFS

For now, DFS performed in following interface:
```c++
void TraverseGraphInDfsOrder(const GraphType &graph, [order], Visitor visitor);
```

Where visitor should have following interface:
```c++
struct DfsVisitor {
    void StartVertex(Node node);
    void DiscoverVertex(Node node);
    void FinishVertex(Node node);
};
```

But this library comes along with fancy extensions. 

At first, you have basic `DfsVisitor` class based on [https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern](CRTP), where you don't need to pay for virtual dispatch, but you do have stable interface ensures you have called your functions in proper way. Moreover, this basic class have useful `Visit(graph[, order])` methods you could use.

At second, you'll love `LamdaVisitor` derived class --- it takes all advantages of static dispatch performed by `DfsVisitor`, but allows you to provide functors for all events in way you love. It could be lambda, fuctor-object, c-style-function, etc. Moreover, you could chain it with 
