#pragma once

#include <cmath>

class Float {
 public:
  static double kPrecision;

  Float(int value, int = 0) : value_(value) {}
  Float(double value = 0, int = 0) : value_{value} {}
  explicit operator double() const { return value_; }

  Float &operator-() { return value_ *= -1, *this; }

  Float &operator+=(const Float &rhs) { return value_ += rhs.value_, *this; }
  Float &operator-=(const Float &rhs) { return value_ -= rhs.value_, *this; }
  Float &operator*=(const Float &rhs) { return value_ *= rhs.value_, *this; }
  Float &operator/=(const Float &rhs) { return value_ /= rhs.value_, *this; }

  friend bool operator< (const Float &lhs, const Float &rhs)
  { return lhs.value_ + kPrecision < rhs.value_; }

  friend bool operator> (const Float &lhs, const Float &rhs)
  { return lhs.value_ - kPrecision > rhs.value_; }

  bool operator==(const Float &rhs) const { return !(*this < rhs) && !(rhs < *this); }
  bool operator!=(const Float &rhs) const { return !(*this == rhs); }

 private:
  double value_;
};

double Float::kPrecision = 1e-9;

Float operator+(const Float &lhs, const Float &rhs) { return Float(lhs) += rhs; }
Float operator-(const Float &lhs, const Float &rhs) { return Float(lhs) -= rhs; }
Float operator*(const Float &lhs, const Float &rhs) { return Float(lhs) *= rhs; }
Float operator/(const Float &lhs, const Float &rhs) { return Float(lhs) /= rhs; }

