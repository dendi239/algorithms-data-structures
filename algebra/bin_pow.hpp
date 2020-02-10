#include <cstdint>

template <class T>
auto BinPow(T base, int64_t exponent) -> T {
    if (exponent == 1) return base;
    if (exponent & 1) return base * BinPow(base * base, exponent >> 1);
    return BinPow(base * base, exponent >> 1);
}
