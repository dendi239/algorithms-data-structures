#pragma once

#include <vector>

namespace fft {

namespace detail {

int reverse(int number, int logs) {
  int res = 0;
  for (int log = 0, rev_log = logs - 1; log < logs; ++log, --rev_log) {
    if (number & (1 << log)) {
      res |= 1 << rev_log;
    }
  }
  return res;
}

template <class T>
T root(size_t pow);

template <>
std::complex<long double> root(size_t pow) {
  long double pi = std::acos(-1);
  return {std::cos(2 * pi / pow), std::sin(2 * pi / pow)};
}

}  // namespace detail

template<class T, class It>
void FFT(It begin, It end, T base) {
  const int size = static_cast<int>(std::distance(begin, end));
  int logs = __builtin_ctz(size);

  for (int index = 0; index < size; ++index) {
    auto rev = detail::reverse(index, logs);
    if (rev < index) {
      std::swap(*(begin + index), *(begin + rev));
    }
  }

  std::vector<T> pows{base};
  for (int i = 0; i + 1 < logs; ++i) {
    pows.push_back(pows.back() * pows.back());
  }

  for (int log = 0; log < logs; ++log) {
    auto bit = 1 << log;
    T mult = pows[logs - log - 1];

    for (int start = 0; start < size; start += 2 * bit) {
      T pow = 1;
      for (int index = start; index < start + bit; ++index) {
        T lhs = *(begin + index), rhs = *(begin + index + bit) * pow;
        *(begin + index) = lhs + rhs, *(begin + index + bit) = lhs - rhs;
        pow *= mult;
      }
    }
  }
}

template <class T>
std::vector<T> multiply(std::vector<T> lhs, std::vector<T> rhs) {
  auto size = lhs.size() + rhs.size() - 1;
  size = 1 << (32 - __builtin_clz(size));

  lhs.resize(size), rhs.resize(size);
  auto base = detail::root<T>(size);

  FFT(lhs.begin(), lhs.end(), base), FFT(rhs.begin(), rhs.end(), base);
  for (size_t index = 0; index < size; ++index) { lhs[index] *= rhs[index]; }

  FFT(lhs.begin(), lhs.end(), T(1) / base);
  for (auto &value : lhs) { value /= size; }

  return lhs;
}

}  // namespace fft