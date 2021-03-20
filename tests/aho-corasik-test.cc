#include "../ranges/iterator_range.hpp"
#include "../graphs/bfs.hpp"
#include "../strings/aho-corasick.hpp"

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <set>
#include <sstream>

void TestAhoCorasick();

int main() {
    TestAhoCorasick();
    std::cerr << "Aho-corasik tests passed!" << std::endl;
}

// ===== TESTING ZONE =====

template<class T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vector) {
    std::copy(vector.begin(), vector.end(), std::ostream_iterator<T>(os, " "));
    return os;
}

class TestNotPassedException : public std::runtime_error {
public:
    explicit TestNotPassedException(const char *what)
        : std::runtime_error(what) {}

    explicit TestNotPassedException(const std::string &what)
        : std::runtime_error(what.c_str()) {}
};

#define REQUIRE_EQUAL(first, second)                                               \
    do {                                                                           \
        auto firstValue = (first);                                                 \
        auto secondValue = (second);                                               \
        if (!(firstValue == secondValue)) {                                        \
            std::ostringstream oss;                                                \
            oss << "Require equal failed: " << #first << " != " << #second << " (" \
                    << firstValue << " != " << secondValue << ")\n";               \
            throw TestNotPassedException(oss.str());                               \
        }                                                                          \
    } while (false)

void TestAhoCorasick() {
    {
        aho_corasick::AutomatonBuilder builder;
        builder.Add("suffix", 1);
        builder.Add("ffix", 2);
        builder.Add("ix", 3);
        builder.Add("abba", 4);

        std::unique_ptr<aho_corasick::Automaton> automaton = builder.Build();

        const std::string text = "let us find some suffix";
        aho_corasick::NodeReference node = automaton->Root();
        for (char ch : text) {
            node = node.Next(ch);
        }
        std::vector<size_t> string_ids;

        node.GenerateMatches(
            [&string_ids](size_t string_id) { string_ids.push_back(string_id); });
        std::sort(string_ids.begin(), string_ids.end());

        REQUIRE_EQUAL(string_ids, std::vector<size_t>({1, 2, 3}));
    }
    {
        aho_corasick::AutomatonBuilder builder;
        builder.Add("", 1);
        builder.Add("", 2);
        builder.Add("t", 3);
        builder.Add("", 4);

        auto automaton = builder.Build();
        const std::string text = "test";
        aho_corasick::NodeReference node = automaton->Root();

        std::vector<size_t> sizes{4, 3, 3, 4};
        for (size_t i = 0; i < text.size(); ++i) {
            node = node.Next(text[i]);
            std::set<int> matches;
            node.GenerateMatches([&matches](size_t id) {
                matches.insert(id);
            });
            REQUIRE_EQUAL(sizes[i], matches.size());
        }
    }
}

