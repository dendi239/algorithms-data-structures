# Graphs

## Representations

Currently, in initial commit only pretty raw interface with adjacency list is supported. Moreover, only one concrete
 graph representation is allowed for undirected unweighted graph.

### TODO
- [x] support graph_traits for any graph representation
    - [x] add default (aka 2-dim vector) graph support
    - [ ] add necessary builders for easy reading/constructing
    - [ ] extend graph traits to work with weight algorithms
- [ ] add weighted graph data structure (or necessary traits for those)
- [ ] ensure present algorithms work fine for all of them
- [ ] unify presented visitors
    - [ ] provide strict list of events
    - [ ] add inheritance based visitor
    - [ ] add CRTP based visitor

## Infrastructure

### Graph Traits

Here we have some bunch of algorithms performing any kinda of traverses around of graph. 
There're many of entities we wanna work as graph and don't wanna to modify them
in order to achieve all profit present graph micro-library could provide.

Hence, we need or graph to define somehow two types `NodeType` , `EdgeType` 
such that following functions are available:
```c++
auto GetTarget(const Graph &graph, const EdgeType &edge) -> NodeType;
auto OutgoingEdges(const Graph &graph, NodeType node) -> Edges;
```
where `Edges` is iterable sequence of edges. Notice that we don't need to store it 
on our own, but this might change, so it would be great for you to provide
`IteratorRange` onto necessary range instead.

Several algorithms might need to color nodes somehow. To maintain that we could 
use `array`, `vector`, or `unordered_set`. For now, `unordered_set` is used, but
I need to provide extension point for user to provide desired data structure such
as `std::vector` to reduce `unordered_set`'s overhead. But for now, there's no 
such extension point, so we might need you to make `NodeType` hashable.

### Visitor

In order to give full access to algorithm without force them to rewrite them ad-hoc, 
we use visitor-pattern. For now, there're `LambdaVisitor` and `BfsVisitor` with 
different events applicable for both of algorithms. 

Note that `Visitor` might be copied inside algorithm, so it's better to make it 
state cheap-to-copy. For now, all algorithms will call all methods of visitor 
on same instance, but this might change.

## Algorithms

Currently, there's DFS presented. It's planned for other algorithms to appear in near future. 
Along with examples.

TODO List for algorithms:
- [X] DFS
    - [ ] Finding strongly-connected-component
    - [ ] Topological sort
    - [ ] Finding bridges
    - [x] Condensation
- [X] BFS
- [ ] Dijkstra
- [ ] Ford-Belman 
- [ ] Complete list of algorithms
- [ ] Lca
