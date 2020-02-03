#include <iostream>
#include <vector>

#include "ranges/ranges.hpp"

using ranges::iota;
using ranges::transform;
using ranges::filter;

bool is_prime(int p) {
    for (auto d : iota(2, p)) {
        if (p % d == 0) return false;
        if (d * d > p) break;
    }
    return p > 1;
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

    auto to_square = [](int &x) { x *= x; return x; };

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (auto x : numbers | filter(is_prime) | transform(to_square)) continue;

    std::cout << "Square for primes, pure for other:";

    for (auto x : numbers) {
        std::cout << " " << x;
    }
    std::cout << '\n';
}
