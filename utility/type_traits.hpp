#pragma once

#include <type_traits>

namespace type_traits {

template <class T>
struct RemoveCVRef {
  using Type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};

template <class T>
using RemoveCVRefT = typename RemoveCVRef<T>::Type;

/**
 * This is sample structure to be used in traits expressions to replace
 * necessary arguments you can't express type yet. See GraphTraits for details.
 */
struct Placeholder {
  template <class T>
  operator T();
};

}  // namespace type_traits
