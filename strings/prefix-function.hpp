#include <string>
#include <vector>

template <class String>
auto prefix_function(const String &text) -> std::vector<int> {
    auto values = std::vector<int>(text.size() + 1);
    
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
