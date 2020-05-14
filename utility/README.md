# Utility

## `class Enumerator<Value, Collection>`

This class allows you to index all elements with consecutive natural (starting with zero) numbers. `Collection` must be collection with iterators and `.find(key) ` that returns iterator comparable with `.end()`.

You're allowed to use whatever collection you need, e.g. `std::map<Value, Compare>`, but all operations with enumerator will cost as much as `Collection` requires for `.find(key)` and `.insert()`.



## `auto make_vec(T, Args...)`

Creates multidimensial vector with given dimensions. Could be handy for some kind of dynamic-programming:

```cpp
auto answers = make_vec(-1_z, 2, 10, 20);
answers[1][5][19] = 100;
```



## `namespace io`

Present namespace contains useful wrappers for reading entities, currently collections:

* `class ContainerReader<It>` reads all values into given range `[begin, end)`. Have ctor by `Container &` with partial deduction guides for `Vec<T>`
* `class SizedVectorReader<T>` reads size of `Vec` at first and fills all elements after that.

