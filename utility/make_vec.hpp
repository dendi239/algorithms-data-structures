#include <vector>

template <class T, class ...Args>
auto make_vec(T default_value, Args ...args);

template <class T>
auto make_vec(T default_value) {
  return default_value;
}

template <class T, class Arg, class ...Args>
auto make_vec(T default_value, Arg arg, Args ...args) {
  return std::vector(arg, make_vec(default_value, args...));
}
