#include <iostream>

namespace io {

template<class It>
struct ContainerReader {
  It begin, end;

  ContainerReader() = default;
  ContainerReader(It begin, It end) : begin(begin), end(end) {}

  template<class T>
  explicit ContainerReader(T &container)
      : begin(std::begin(container)), end(std::end(container))
  {}
};

template <class T>
ContainerReader(std::vector<T> &vector) -> ContainerReader<typename std::vector<T>::iterator>;

template <class It>
std::istream &operator>>(std::istream &is, const ContainerReader<It> &container_reader) {
  for (auto it = container_reader.begin; it != container_reader.end; ++it) {
    is >> *it;
  }
  return is;
}

template <class T>
struct SizedVectorReader {
  std::vector<T> &vector;

  explicit SizedVectorReader(std::vector<T> &vector) : vector(vector) {}
};

template <class T>
std::istream &operator>>(std::istream &is, const SizedVectorReader<T> &vector_reader) {
  int size;
  is >> size;

  vector_reader.vector.resize(size);
  return is >> ContainerReader(vector_reader.vector);
}

}  // namespace io

