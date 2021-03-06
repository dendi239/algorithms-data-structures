#pragma once

#include <cmath>

class Float {
 public:
  static double kPrecision;

//  Float(int value, int = 0) : value_(value) {}
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
  friend istream &operator>>(istream &os, Float& value);
  friend ostream &operator<<(ostream &os, const Float& value);

  double value_;
};

double Float::kPrecision = 1e-9;

Float operator+(const Float &lhs, const Float &rhs) { return Float(lhs) += rhs; }
Float operator-(const Float &lhs, const Float &rhs) { return Float(lhs) -= rhs; }
Float operator*(const Float &lhs, const Float &rhs) { return Float(lhs) *= rhs; }
Float operator/(const Float &lhs, const Float &rhs) { return Float(lhs) /= rhs; }

Float operator""_f(unsigned long long value) {
  return Float(value);
}

Float operator""_f(long double value) {
 return Float(value); 
}

ostream &operator<<(ostream &os, const Float& value)
{ return os << static_cast<double>(value); }

istream &operator>>(istream &is, Float& value)
{ return is >> value.value_; }
