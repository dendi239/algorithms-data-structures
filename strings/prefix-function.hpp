#include <string>
#include <vector>

auto prefix_function(const std::string &text) -> std::vector<size_t> {
    auto values = std::vector<size_t>(text.size() + 1);
    
    for (size_t index = 1; index < text.size(); ++index) {
        for (size_t start = index, length; length = values[start],
             start > 0; start = values[start]) {
            if (text[index] == text[length]) {
                values[index + 1] = length + 1;
                break;
            }
        }
    }
    
    return values;
}
