#include "iterator_range.hpp"
#include "container_adapter.hpp"

namespace ranges {

namespace detail {

template <class Iterator, class Pred>
struct FilterIteratorAdapter {
  FilterIteratorAdapter(Iterator it, Iterator end, Pred pred) 
  : it_(it), end_(end), pred_(pred) {}

  FilterIteratorAdapter &operator++() {
    do { 
      ++it_;
    } while (it_ != end_ && !pred_(*it_));
    return *this;
  }

  auto &operator*() const {
    return *it_;
  }

  bool operator!=(const FilterIteratorAdapter &other) const {
    return it_ != other.it_;
  }

private:
  Iterator it_, end_;
  Pred pred_;
};

template <class Pred>
struct Filter : ContainerAdapter<Filter<Pred>> {
  explicit Filter(Pred pred) : pred_(pred) {}

  template <class Container> 
  auto operator()(Container &container) const {
    auto begin = FilterIteratorAdapter(
      container.begin(), container.end(), pred_);
    auto end = FilterIteratorAdapter(
      container.end(), container.end(), pred_);
    return IteratorRange(begin, end);
  }

  template <class Container>
  auto operator()(const Container &container) const {
    auto begin = FilterIteratorAdapter(
      container.begin(), container.end(), pred_);
    auto end = FilterIteratorAdapter(
      container.end(), container.end(), pred_);
    return IteratorRange(begin, end);
  }
  
private:
  Pred pred_;
};

} // namespace detail

template <class Pred>
auto filter(Pred pred) {
  return detail::Filter(pred);
}

} // namespace ranges