# Algorithms and Data structures

This is library of algrotihms and data-structures frequently used in 
different competitive programming problems. It follows DRY principle to reduce
amount of time spent on writing known solutions and concentrate on new ones.

It's divided into few subfolders:

### Algebra 

There're all stuff you'll need from algebra world. For now, it contains 
simple algorithms such as BinPow, BinSearch, way to operate moduled numbers, 
counting Cs and that's all for now.

### Data Structures 

There's concentrated all necessary data structures such as RMQ, DSU, or Partition
used in Condensation algorithm. It's planned to add SegmentTree or Treap as well, 
but it has more detailed design to develop at first.

### Graphs 

There're a lot of graphs algortihms designed to work with any graph you'll have.
In order to apply same algorithms there's graph_traits used to unify all graphs
to have common interface for graphs' and algorithms' suppliers. There're only 
unweighted graphs supported, but there're bunch of algorithms using weights 
may be unified as well.

### Strings

There're few strings algorithms and data structures representing strings.
They use graph's library much enough, but doesn't have deep design. E.g., both
of `SuffAutomaton` and `Ukkonen` has methods `Append` and `Contains`, but don't 
have much in common beside simmilar interface.

### Ranges

This is toy ranges library with convenient operators inspired by ranges library 
comes in c++20.

## Contributions

Contributions are welcome! There're bunch of requirements:
- [Google C++ Code Style Guide](https://google.github.io/styleguide/cppguide.html) is required
- Follow RAII principles: avoid methods init, cleanup and others
- Don't create global variables: users may be forced to exclude your header in order to reduce memory use
- Try to avoid magic constants such as `MAX_N`: 
 use as much resources as much you really need to make all usages more flexible,
 e.g. users may wanna to create a lot of instances at same time while you're 
 focusing on single one. 
