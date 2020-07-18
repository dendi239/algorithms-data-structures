# Algebra stuff

## `T BinPow(T, int64_t)`

This is simple binary pow algorithm, works with any `T` capable of multiplying (aka `operator*`), doesn't require neutral element to work, since that works only with positive exponents.



## `T BinSearch(T, T, P)`

Simple binary search algorithm. Works with predicate `P` that's true for some `[l, m]` and false for `(m, r)`. Returns `m` --- last true value. It's use `operator!=` to compare if it's finished, so you allowed to pass `l > r`, or even _class without `operator<` defined_.

It works nicely with integers, to work with floating point values, see `class Float`.



## FFT

Applyies fast fourier transform to given range with given base. Works nicely with any data types that have ctor from integers and `base ^ size == 1`. 

Function `multiply(Vec<T>, Vec<T>)` supposed to work fine with any data types such as `complex<double>` or `Z<mod>`, but currently `Polynomial<T>::operator*` uses `llround` and there's missing `root<T>(size_t size)` for any other `T != complex<Double>`.

### TODO:

- [ ] support `root<Z<mod>>(size_t)`
- [ ] adopt `Polynomial<T>::operator*` to work fine with `Z<mod>`
- [ ] add `Polynomial` section



## `class C<T>`

Counts all necessary cs with given size. Stores factorials and performs exactly one division, so works the best with `Z<mod>`, but may be used with floats as well.



## `class Modular<Modulo>`

Class [`Modular<Modulo>`](modular.hpp) incapsulates residue by given modulo. 
Works fine with all operators you might need, moreover you're allowed to pass any `Modulo` you might need work with.
If you need to work with `operator/` you might need to implement `is_invertible` trait:
```c++
struct is_invertible<YourModulo> {
  static bool value = true;
  
  // Type there is `std::remove_cv_t<decltype(YourModulo::value)>
  static Type inverse(Type number) {
    // your implementation goes here
  }
};
```

Currently supported: 
- constructor, `operator Type`
- `operator+=`, `-=`, `*=`, `/=`, `==`, `!=`, `<`, `++`, `--`
- `operator>>` for `istream` and `operator<<` for `ostream`

So you can operate with `Modulars`s like any other numbers without need to worry about performing operations by modulo at any specific point.
However, it might be annoying to provide single class to work with it. 
So, there're two concrete classes with useful type-aliases, take a look:

```c++
using Mint = Z<1'000'000'009>;
// deal with Mint as ints at this point

using Mint = DynamicModular<int64_t>;
// fill Mint::Modulo::value somehow before use -- by default it equals 0 
```

## `class Float`

This class is arifmetical class that incapsulates `double` values with fixed precision for comparision. It's **not** deriving linear order, since `a == b`, `b == c` doesn't derive `a == c` since `abs(a - c) < 2 * kPrecision` for sure. 



Beyond that you're capable of using it wherever you need, be sure to setup `Float::kPresicion` at first. By default it equals to `1e-9`.
