#pragma once

#include <complex>
#include <vector>

#include <algebra/fft.hpp>

template <class T>
class Polynomial {
 public:
  explicit Polynomial(size_t degree = 0) : coefficients_(degree) {}

  template <class It> Polynomial(It begin, It end) : coefficients_(begin, end) {}

  explicit Polynomial(std::initializer_list<T> initializer_list)
      : coefficients_(std::move(initializer_list))
  {}

  size_t degree() const { return coefficients_.size(); }
  void resize(size_t degree) const { coefficients_.resize(degree); }

  auto &operator[](size_t pow) { return coefficients_[pow]; }
  const auto &operator[](size_t pow) const { return coefficients_[pow]; }

  auto operator()(T point) const {
    T result = 0, mult = 1;
    for (auto coeff : coefficients_) {
      result += mult * coeff;
      mult *= point;
    }
    return result;
  }

  Polynomial &operator+=(const Polynomial &rhs);
  Polynomial &operator-=(const Polynomial &rhs);

 private:
  template <class U>
  friend Polynomial<U> operator*(const Polynomial<U> &lhs, const Polynomial<U> &rhs);

  std::vector<T> coefficients_;
};

template <class T>
Polynomial<T> operator*(const Polynomial<T> &lhs, const Polynomial<T> &rhs) {
  using Num = std::complex<long double>;
  auto lhs_coefficients = std::vector<Num>(lhs.coefficients_.begin(), lhs.coefficients_.end());
  auto rhs_coefficients = std::vector<Num>(rhs.coefficients_.begin(), rhs.coefficients_.end());

  auto result = fft::multiply(lhs_coefficients, rhs_coefficients);
  auto result_polynomial = Polynomial<T>(result.size());

  for (size_t i = 0; i < result.size(); ++i) {
    result_polynomial[i] = llround(result[i].real());
  }

  return result_polynomial;
}

template <class T>
Polynomial<T> &Polynomial<T>::operator+=(const Polynomial &rhs) {
  for (size_t pow = 0; pow < std::min(degree(), rhs.degree()); ++pow) {
    coefficients_[pow] += rhs[pow];
  }
  return *this;
}
