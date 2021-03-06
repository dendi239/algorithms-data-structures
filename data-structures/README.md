#  Disjoint set union

This is simple data structure represents disjoint sets over elements `0`, ..., `n-1` where `n` is size of entire
 collection. If you find it's annoying to build indices over elements you have, take a look to `Compresser`: 

```c++
void merge(int lhs, int rhs);
bool is_same(int lhs, int rhs) const;
```

# Range minimum query


