#pragma once

#include <iostream>
#include <utility>
#include <type_traits>

template <int64_t mod>
class Z {
 public:
  template <
      class Int = int,
      class = typename std::enable_if<std::is_integral<Int>::value>::type>
  Z(Int value = 0, int = 0) : value_(value % mod) {}

  template <
      class Int = int,
      class = typename std::enable_if<std::is_integral<Int>::value>::type>
  explicit operator Int() const { return (value_ + mod) % mod; }

  Z Inverse() const;

  Z &operator+=(const Z &other);
  Z &operator-=(const Z &other);
  Z &operator*=(const Z &other);
  Z &operator/=(const Z &other);

  bool operator==(const Z &other) const;
  bool operator!=(const Z &other) const;

  bool operator<(const Z &other) const;

 private:
  int value_;
};

template <int64_t mod>
std::istream &operator>>(std::istream &is, Z<mod> &z) {
  int64_t value;
  is >> value;
  z = Z<mod>(value);
  return is;
}

template <int64_t mod>
std::ostream &operator<<(std::ostream &os, const Z<mod> &z) {
  return os << static_cast<int64_t>(z);
}

template <int64_t mod>
bool Z<mod>::operator<(const Z<mod> &rhs) const {
  return ((value_ + mod) % mod) < ((rhs.value_ + mod) % mod);
}

template <int64_t mod>
bool Z<mod>::operator==(const Z &other) const {
  return (value_ - other.value_) % mod == 0;
}

template <int64_t mod>
bool Z<mod>::operator!=(const Z &other) const {
  return (value_ - other.value_) % mod != 0;
}

std::pair<int64_t, int64_t> LrpGcd(int64_t first, int64_t second) {
  if (second == 0) {
    if (first > 0) {
      return {+1, 0};
    } else {
      return {-1, 0};
    }
  }

  auto [prev_first_coeff, prev_second_coeff] = LrpGcd(second, first % second);
  return {
      prev_second_coeff,
      prev_first_coeff - (first / second) * prev_second_coeff,
  };
}

template <int64_t mod>
Z<mod> Z<mod>::Inverse() const {
  auto lrp_gcd = LrpGcd(value_, mod);
  return Z(lrp_gcd.first);
}

template <int64_t mod>
Z<mod> &Z<mod>::operator+=(const Z &other) {
  value_ += other.value_;
  if (value_ >= +mod) value_ -= mod;
  if (value_ <= -mod) value_ += mod;
  return *this;
}

template <int64_t mod>
Z<mod> &Z<mod>::operator-=(const Z &other) {
  value_ -= other.value_;
  if (value_ >= +mod) value_ -= mod;
  if (value_ <= -mod) value_ += mod;
  return *this;
}

template <int64_t mod>
Z<mod> &Z<mod>::operator*=(const Z &other) {
  value_ = static_cast<int64_t>(value_) * other.value_ % mod;
  return *this;
}

template <int64_t mod>
Z<mod> &Z<mod>::operator/=(const Z &other) {
  return *this *= other.Inverse();
}

template <int64_t mod>
Z<mod> operator+(const Z<mod> &lhs, const Z<mod> &rhs) {
  return Z<mod>(lhs) += rhs;
}

template <int64_t mod>
Z<mod> operator-(const Z<mod> &lhs, const Z<mod> &rhs) {
  return Z<mod>(lhs) -= rhs;
}

template <int64_t mod>
Z<mod> operator*(const Z<mod> &lhs, const Z<mod> &rhs) {
  return Z<mod>(lhs) *= rhs;
}

template <int64_t mod>
Z<mod> operator/(const Z<mod> &lhs, const Z<mod> &rhs) {
  return Z<mod>(lhs) /= rhs;
}
