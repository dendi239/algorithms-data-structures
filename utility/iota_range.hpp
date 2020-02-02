#include "iterator_range.hpp"
#include "container_adapter.hpp"

namespace ranges {

namespace detail {

template <class T>
struct IotaIterator {
  explicit IotaIterator(T value = T()) : value_(value) {}

  IotaIterator &operator++() {
    ++value_;
    return *this;  
  }

  const T &operator*() const {
    return value_;
  }

  bool operator!=(const IotaIterator &rhs) const {
    return value_ != rhs.value_;
  }

private:
  T value_;
};

} // namespace detail

template <class T>
auto iota(T from, T to) {
  auto begin = detail::IotaIterator(from);
  auto end = detail::IotaIterator(to);
  return detail::IteratorRange(begin, end);
}

template <class T> 
auto iota(T to) {
  return iota(T(), to);
}

} // namespace ranges
