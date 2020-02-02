#pragma once

template <class concrete>
struct ContainerAdapter {
  template <class Container>
  auto operator()(const Container &container) const {
    return static_cast<const concrete &>(*this)(container);
  }

  template <class Container>
  auto operator()(Container &container) const {
    return static_cast<const concrete &>(*this)(container);
  }
};

template <class Adapter, class Container> 
auto operator|(
  const Container &container,
  const ContainerAdapter<Adapter> &adapter) {
  return adapter(container);
}

template <class Adapter, class Container>
auto operator|(
  Container &container,
  const ContainerAdapter<Adapter> &adapter) {
  return adapter(container);
}
