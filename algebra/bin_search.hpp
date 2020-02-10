template <class T, class F>
auto BinSearch(T left, T right, F func) -> T {
  for (T mid; mid = (left + right) / 2, mid != left && mid != right;) {
    (func(mid) ? left : right) = mid;
  }
  return left;
}