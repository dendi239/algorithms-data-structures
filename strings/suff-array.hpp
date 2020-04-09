#pragma once

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

struct SuffArrayInfo {
  std::vector<int> suff_array;
  std::vector<int> lcp_array;
};

auto SuffArray(std::string_view str, bool cyclic = false) -> SuffArrayInfo {
  const int text_size = str.size();
  std::vector<int> poses(text_size);
  std::vector<std::vector<int>> all_ranks{std::vector<int>(text_size)};

  {
    auto &ranks = all_ranks.front();
    std::iota(poses.begin(), poses.end(), 0);
    std::sort(poses.begin(), poses.end(), [&](int lhs, int rhs) {
      return str[lhs] < str[rhs];
    });
    for (int i = 1; i < text_size; ++i) {
      ranks[poses[i]] = ranks[poses[i - 1]] + (str[poses[i]] != str[poses[i - 1]]);
    }
  }

  std::vector<int> next_pos(text_size);
  std::vector<int> starts(text_size + 1);
  std::vector<int> new_ranks(text_size);

  int max_log = 0;
  for (int log = 0, exp = 1; exp < text_size; max_log = ++log, exp <<= 1) {
    auto &ranks = all_ranks.back();

    for (int i = 0; i < text_size; ++i) { starts[i] = 0; }
    for (int i = 0; i < text_size; ++i) { ++starts[ranks[i] + 1]; }
    for (int i = 0; i < text_size; ++i) { starts[i + 1] += starts[i]; }

    if (!cyclic) {
      for (auto pos : poses) {
        if (pos < exp) {
          auto location = starts[ranks[pos - exp + text_size]]++;
          next_pos[location] = pos - exp + text_size;
        }
      }
      for (auto pos : poses) {
        if (pos >= exp) {
          auto location = starts[ranks[pos - exp]]++;
          next_pos[location] = pos - exp;
        }
      }
    } else {
      for (auto pos : poses) {
        auto index = (pos + text_size - exp) % text_size;
        auto location = starts[ranks[index]]++;
        next_pos[location] = index;
      }
    }

    new_ranks[next_pos[0]] = 0;
    for (int index = 1; index < text_size; ++index) {
      new_ranks[next_pos[index]] = new_ranks[next_pos[index - 1]];
      if (cyclic) {
        auto next = (next_pos[index] + exp) % text_size;
        auto next_prev = (next_pos[index - 1] + exp) % text_size;
        if (ranks[next_pos[index]] != ranks[next_pos[index - 1]] ||
            ranks[next] != ranks[next_prev]) {
          ++new_ranks[next_pos[index]];
        }
        continue;
      }
      if (ranks[next_pos[index]] != ranks[next_pos[index - 1]] ||
          next_pos[index] >= text_size - exp ||
          next_pos[index - 1] >= text_size - exp ||
          ranks[next_pos[index] + exp] != ranks[next_pos[index - 1] + exp]) {
        ++new_ranks[next_pos[index]];
      }
    }

    poses = next_pos;
    all_ranks.push_back(new_ranks);

    if (new_ranks[next_pos.back()] == text_size) break;
  }

  std::vector<int> lcp(text_size);
  for (int suff_index = 0; suff_index + 1 < text_size; ++suff_index) {
    int lhs = poses[suff_index], rhs = poses[suff_index + 1];
    for (int log = max_log; lhs < text_size && rhs < text_size && log >= 0; --log) {
      if (all_ranks[log][lhs] == all_ranks[log][rhs]) {
        lhs += 1 << log, rhs += 1 << log;
        lcp[suff_index] += 1 << log;
      }
    }
  }

  return {poses, lcp};
}
