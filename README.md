# Algorithms and Data structures

This is a library of algorithms and data structures frequently used in 
different competitive programming problems. It follows DRY principle to reduce 
the amount of time spent on writing already known solutions and concentrate on new ones.

It's divided into few subfolders:

### Algebra 

There's all the stuff you'll need from algebra world. For now, it contains 
simple algorithms such as BinPow, BinSearch, modular arithmetic, 
counting Cs and that's all for now.

### Data Structures 

All necessary data structures such as RMQ, DSU,
or Partition used in Condensation algorithm are concentrated here.
It's planned to add SegmentTree or Treap as well, 
but it has more detailed design to develop at first.

### Graphs 

There're a lot of graphs algorithms designed to work with any graph you'll have.
In order to apply same algorithms there's graph_traits used to unify all graphs
to have common interface for graphs' and algorithms' suppliers. There're only 
unweighted graphs supported, but there're bunch of algorithms using weights 
may be unified as well.

### Strings

There're a few strings algorithms and data structures representing strings.
They use graph's library much enough, but don't have deep design. E.g., both
of `SuffAutomaton` and `Ukkonen` have methods `Append` and `Contains`, but don't 
have much in common besides similar interface.

### Ranges

This is toy ranges library with convenient operators inspired by ranges library 
coming in c++20.

## Contributions

Contributions are welcome! There're bunch of requirements:
- [Google C++ Code Style Guide](https://google.github.io/styleguide/cppguide.html) is required
- Follow RAII principles: avoid methods init, cleanup and others
- Don't create global variables: users may be forced to exclude your header in order to reduce memory use
- Try to avoid magic constants such as `MAX_N`: 
 use as much resources as much you really need to make all usages more flexible,
 e.g. users may wanna to create a lot of instances at same time while you're 
 focusing on single one. 
