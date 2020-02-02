#include "iterator_range.hpp"
#include "container_adapter.hpp"

namespace ranges {

namespace detail {

template <class Iterator, class Transform>
struct TransformIteratorAdapter {
  TransformIteratorAdapter(Iterator it, Transform transform)
  : it_(it), transform_(transform) {}

  TransformIteratorAdapter &operator++() {
    return ++it_, *this;
  }

  auto operator*() const {
    return transform_(*it_); 
  }

  bool operator!=(const TransformIteratorAdapter &other) const {
    return it_ != other.it_;
  }

private:
  Iterator it_;
  Transform transform_;
};


template <class Func>
struct Transform : ContainerAdapter<Transform<Func>> {
  explicit Transform(Func func) : func_(func) {}

  template <class Container>
  auto operator()(const Container &container) const {
    auto begin = TransformIteratorAdapter(container.begin(), func_);
    auto end = TransformIteratorAdapter(container.end(), func_);
    return IteratorRange(begin, end);
  }

  template <class Container>
  auto operator()(Container &container) const {
    auto begin = TransformIteratorAdapter(container.begin(), func_);
    auto end = TransformIteratorAdapter(container.end(), func_);
    return IteratorRange(begin, end);
  }

private:
  Func func_;
};

} // namespace detail

template <class Func>
auto transform(Func func) {
  return detail::Transform(func);
}

} // namespace ranges