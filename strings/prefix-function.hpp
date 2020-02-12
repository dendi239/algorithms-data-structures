#include <string>
#include <vector>

class PrefixFunciton {
 public:
  explicit PrefixFunciton(const std::string &str)
  : text(str), values(str.size() + 1) {
    for (size_t i = 1; i < str.size(); ++i) {
      if (str[0] == str[1]) values[i + 1] = 1;
      for (size_t j = i, lenght; lenght = values[j], j > 0; j = values[j]) {
        if (str[i] == str[lenght]) {
          values[i + 1] = lenght + 1;
          break;
        }
      }
    }
  }

  auto operator()(size_t prefix_size) const -> size_t {
    return values[prefix_size];
  }

 private:
  std::string text;
  std::vector<size_t> values;
};
