#pragma once

#include <istream>
#include <ostream>
#include <utility>

template<class ValueType>
struct is_invertible {
  static constexpr bool value = false;
};

template<class Modulo>
class Modular {
  using Z = Modular;
  using Type = std::remove_cv_t<decltype(Modulo::value)>;

 public:
  using ModuloType = Modulo;

  Modular(Type value = {}) : value_{value} {}
  explicit operator Type() const { return (Mod() + value_) % Mod(); }

  Z &operator+=(const Z &rhs) { return value_ += rhs.value_, Normalize(); }
  Z &operator-=(const Z &rhs) { return value_ -= rhs.value_, Normalize(); }
  Z &operator*=(const Z &rhs) { return value_ *= rhs.value_, Normalize(); }

  template<std::enable_if_t<is_invertible<Modulo>::value, int> = 0>
  Z &operator/=(const Z &rhs) {
    value_ *= is_invertible<Modulo>::inverse(rhs.value_);
    return Normalize();
  }

  Z operator-() const { return Z{-value_}; }

  Z &operator++() { return ++value_, Normalize(), *this; }
  Z &operator--() { return --value_, Normalize(), *this; }

  bool operator<(const Z &rhs) const {
    return static_cast<Type>(*this) < static_cast<Type>(rhs);
  }

  bool operator==(const Z &rhs) const {
    return (value_ - rhs.value_) % Mod() == 0;
  }
  bool operator!=(const Z &rhs) const { return !(*this == rhs); }

  friend Z operator+(const Z &lhs, const Z &rhs) { return Z(lhs) += rhs; }
  friend Z operator-(const Z &lhs, const Z &rhs) { return Z(lhs) -= rhs; }
  friend Z operator*(const Z &lhs, const Z &rhs) { return Z(lhs) *= rhs; }

  template<std::enable_if_t<is_invertible<Modulo>::value, int> = 0>
  friend Z operator/(const Z &lhs, const Z &rhs) { return Z(lhs) /= rhs; }

  friend std::istream &operator>>(std::istream &is, Z &value) {
    return is >> value.value_, value.Normalize(), is;
  }

  friend std::ostream &operator<<(std::ostream &os, const Z &value) {
    return os << static_cast<Type>(value);
  }

 private:
  static Type Mod() { return Modulo::value; }

  Z &Normalize() {
    value_ %= Mod();
    return *this;
  }

  Type value_;
};

template<int64_t mod>
using Z = Modular<std::integral_constant<int64_t, mod>>;

constexpr auto lrp_gcd(int64_t a, int64_t b) -> std::pair<int64_t, int64_t> {
  if (!b) {
    return {1, 0};
  }
  int64_t x = 0, y = 0;
  std::tie(x, y) = lrp_gcd(b, a % b);
  return {
      y,
      x - (a / b) * y,
  };
}

template<int64_t mod>
struct is_invertible<std::integral_constant<int64_t, mod>> {
  static constexpr bool value = true;

  constexpr static int64_t inverse(int64_t number) {
    return lrp_gcd(number, mod).first;
  }
};

template<class T>
struct DynamicModulo {
  static T value;
};

template<class T>
using DynamicModular = Modular<DynamicModulo<T>>;

template<class T>
T DynamicModulo<T>::value;

template<>
struct is_invertible<DynamicModulo<int64_t>> {
  static constexpr bool value = true;

  static int64_t inverse(int64_t number) {
    return lrp_gcd(number, DynamicModulo<int64_t>::value).first;
  }
};
