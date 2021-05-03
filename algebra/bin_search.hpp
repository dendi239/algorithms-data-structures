#pragma once

/**
 * BinSearch performs binary search on interval [left, right) with given predicate func
 *
 * @tparam T -- type of passed point
 * @tparam F -- type of binary predicate, like bool(T)
 * @param left -- begin of searched interval
 * @param right -- end of searched interval
 * @param func -- binary predicate
 * @return last point where func(x) is true
 *
 * @note: you must satisfy func(left) = true
 * @note: predicate should have following semantics:
 * - true  for [left, mid]
 * - false for (mid, right)
 *
 * In this case you'll get `mid` as a result.
 *
 * @note: floating point are supported
 * Basically, there's no additional code to support floating point ranges,
 * however, implementations iterates until mid = (left + right) / 2 differs from
 * both of left and right. Hence, you can use your own class Float with
 * operator+, operator/, operator!= that uses some precision.
 */
template <class T, class F>
auto BinSearch(T left, T right, F func) -> T {
  for (T mid; mid = (left + right) / 2, mid != left && mid != right;) {
    (func(mid) ? left : right) = mid;
  }
  return left;
}
