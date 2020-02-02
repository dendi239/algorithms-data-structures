#include <iostream>
#include "ranges/ranges.hpp"

using ranges::iota;
using ranges::transform;
using ranges::filter;

bool is_prime(int p) {
    for (auto d : iota(2, p)) {
        if (p % d == 0) return false;
        if (d * d > p) break;
    }
    return true;
}

int square(int x) {
    return x * x;
}

auto main() -> int {
    std::cout << "Squares of all primes from 1..100:";

    for (int p : iota(100) | filter(is_prime) | transform(square)) {
        std::cout << ' '  << p;
    }

    std::cout << std::endl;
}