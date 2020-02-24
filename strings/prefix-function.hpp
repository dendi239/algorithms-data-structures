#include <string>
#include <vector>

class PrefixFunction {
 public:
  explicit PrefixFunction(const std::string &str)
  : text(str), values(str.size() + 1) {
    for (size_t index = 1; index < str.size(); ++index) {
      for (size_t start = index, length; length = values[start], 
           start > 0; start = values[start]) {
        if (text[index] == text[length]) {
          values[index + 1] = length + 1;
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
