#include <cstdint>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "lib.hpp"

// Globals
constexpr int sequence_length = 10;
constexpr int max_vowels = 2;

namespace
{
auto is_vowel(const std::string& key) -> bool
{
  return key == "A" || key == "E" || key == "I" || key == "O";
}

auto key_layout() -> const std::vector<std::string>&
{
  static const std::vector<std::string> positions_vec = {"A",
                                                         "B",
                                                         "C",
                                                         "D",
                                                         "E",
                                                         "F",
                                                         "G",
                                                         "H",
                                                         "I",
                                                         "J",
                                                         "K",
                                                         "L",
                                                         "M",
                                                         "N",
                                                         "O",
                                                         "1",
                                                         "2",
                                                         "3"};
  return positions_vec;
}

auto get_key_coords() -> std::unordered_map<std::string, std::pair<int, int>>
{
  return {{"A", {0, 0}},
          {"B", {0, 1}},
          {"C", {0, 2}},
          {"D", {0, 3}},
          {"E", {0, 4}},
          {"F", {1, 0}},
          {"G", {1, 1}},
          {"H", {1, 2}},
          {"I", {1, 3}},
          {"J", {1, 4}},
          {"K", {2, 0}},
          {"L", {2, 1}},
          {"M", {2, 2}},
          {"N", {2, 3}},
          {"O", {2, 4}},
          {"1", {3, 1}},
          {"2", {3, 2}},
          {"3", {3, 3}}};
}

// Reverts the key_layout mapping on the fly, inlined because its a hot function
// We should precompute this if the keyboard layout is larger but for our case
// it is fine
inline auto invert_key_positions(
    const std::unordered_map<std::string, std::pair<int, int>>& key_pos)
    -> std::unordered_map<int, std::unordered_map<int, std::string>>
{
  std::unordered_map<int, std::unordered_map<int, std::string>> pos_keys;
  for (const auto& [key, coords] : key_pos) {
    pos_keys[coords.first][coords.second] = key;
  }
  return pos_keys;
}

auto init_vowels_dp()
    -> std::unordered_map<std::string, std::vector<std::vector<int64_t>>>
{
  std::unordered_map<std::string, std::vector<std::vector<int64_t>>> dp_array;
  for (const std::string& key : key_layout()) {
    // max vowels + 1 to map 0 vowels as well
    dp_array[key] = std::vector<std::vector<int64_t>>(
        sequence_length, std::vector<int64_t>(max_vowels + 1, 0));
  }
  return dp_array;
}

// For every possible knight move from key calc the the new vowel count
void dp_step(
    std::unordered_map<std::string, std::vector<std::vector<int64_t>>>&
        dp_array,
    const std::unordered_map<std::string, std::vector<std::string>>&
        knight_moves,
    std::unordered_map<std::string, std::vector<std::vector<int64_t>>>& next_dp,
    size_t pos)
{
  for (const std::string& key : key_layout()) {
    for (size_t vowel = 0; vowel <= max_vowels; ++vowel) {
      const int64_t count = dp_array[key][pos - 1][vowel];
      if (count == 0) {
        continue;
      }
      for (const std::string& dest : knight_moves.at(key)) {
        const size_t new_vowel = vowel + (is_vowel(dest) ? 1 : 0);
        if (new_vowel > max_vowels) {
          continue;
        }
        next_dp[dest][pos][new_vowel] += count;
      }
    }
  }
}

auto aggregate_dp(
    std::unordered_map<std::string, std::vector<std::vector<int64_t>>> dp_array)
    -> int64_t
{
  int64_t total = 0;
  for (const std::string& key : key_layout()) {
    for (size_t vowel = 0; vowel <= max_vowels; ++vowel) {
      total += dp_array[key][sequence_length - 1][vowel];
    }
  }
  return total;
}

}  // namespace

// Pre compute all knight moves
auto get_knight_moves()
    -> std::unordered_map<std::string, std::vector<std::string>>
{
  auto key_pos = get_key_coords();

  auto pos_keys = invert_key_positions(key_pos);
  std::unordered_map<std::string, std::vector<std::string>> moves;
  // Define possible moves of a Knight i.e., 1 is down, -1 is up, 2 is right, -2
  // is left
  const std::vector<std::pair<int, int>> directions = {
      {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

  for (const auto& [key, coords] : key_pos) {
    const int coord_x = coords.first;
    const int coord_y = coords.second;
    // Calculate possible knight moves
    for (const auto& direction : directions) {
      const int dir_x = coord_x + direction.first;
      const int dir_y = coord_y + direction.second;
      // Check if the outter map contains dir_x if dir_x exists we check dir_y
      if (pos_keys.count(dir_x) != 0U && pos_keys.at(dir_x).count(dir_y) != 0U)
      {
        moves[key].push_back(pos_keys.at(dir_x).at(dir_y));
      }
    }
  }
  return moves;
}

auto count_knight_sequences() -> int64_t
{
  auto knight_moves = get_knight_moves();
  // Map of key to <seq_position, vowel_count> init to 0
  std::unordered_map<std::string, std::vector<std::vector<int64_t>>> dp_array =
      init_vowels_dp();

  // Loop all of key_layout and set vowel to 1 if vowel
  for (const std::string& key : key_layout()) {
    const size_t vowel = is_vowel(key) ? 1 : 0;
    dp_array[key][0][vowel] = 1;
  }

  for (size_t pos = 1; pos < sequence_length; ++pos) {
    std::unordered_map<std::string, std::vector<std::vector<int64_t>>> next_dp =
        init_vowels_dp();
    dp_step(dp_array, knight_moves, next_dp, pos);
    dp_array = next_dp;
  }

  const int64_t total = aggregate_dp(dp_array);
  return total;
}
