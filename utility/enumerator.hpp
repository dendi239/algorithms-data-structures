#pragma once

#include <unordered_map>

template <class T, class Container = std::unordered_map<T, int>>
class Enumerator {
 public:
  int size() const {
    return data_.size();
  }

  int operator()(const T &value) {
    if (auto it = data_.find(value); it != data_.end()) {
      return it->second;
    } else {
      return data_[value] = last_id++;
    }
  }

 private:
  int last_id = 0;
  Container data_;
};

