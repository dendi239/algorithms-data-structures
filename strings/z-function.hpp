#include <algorithm>
#include <string>
#include <vector>

auto z_function(const std::string &text) -> std::vector<int> {
  std::vector<int> values(text.size());
  int start = 0, end = 0;

  for (int index = 1; index < static_cast<int>(text.size()); ++index) {
    int finish = index;

    if (end > index) {
      finish = index + values[index - start];
      finish = std::min(finish, end);
    }

    for (; finish < static_cast<int>(text.size()); ++finish) {
      if (text[finish] != text[finish - index]) {
        break;
      }
    }

    if (end < finish) {
      start = index, end = finish;
    }

    values[index] = finish - index;
  }

  return values;
}
