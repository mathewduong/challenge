#include <cstdint>
#include <cstdlib>
#include <iostream>
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
auto is_vowel(const std::string& letter) -> bool
{
  return letter == "A" || letter == "E" || letter == "I" || letter == "O";
}

auto letter_layout() -> const std::vector<std::string>&
{
  static const std::vector<std::string> letter_vec = {"A",
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
  return letter_vec;
}

auto get_letter_coords() -> std::unordered_map<std::string, std::pair<int, int>>
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

// Invert the letter_layout mapping to quickly lookup the X(row) and Y(col) and
// get the letter
auto invert_letter_layout(
    const std::unordered_map<std::string, std::pair<int, int>>& letter_coords)
    -> std::unordered_map<int, std::unordered_map<int, std::string>>
{
  std::unordered_map<int, std::unordered_map<int, std::string>> coords_letter;
  for (const auto& [letter, coords] : letter_coords) {
    coords_letter[coords.first][coords.second] = letter;
  }
  return coords_letter;
}

auto init_vowels_dp()
    -> std::unordered_map<std::string, std::vector<std::vector<int64_t>>>
{
  std::unordered_map<std::string, std::vector<std::vector<int64_t>>> dp_array;
  for (const std::string& letter : letter_layout()) {
    // max vowels + 1 to map 0 vowels as well
    dp_array[letter] = std::vector<std::vector<int64_t>>(
        sequence_length, std::vector<int64_t>(max_vowels + 1, 0));
  }
  return dp_array;
}

// For every possible knight move from letter, calc the the new vowel count
void dp_step(
    const std::unordered_map<std::string, std::vector<std::vector<int64_t>>>&
        dp_array,
    const std::unordered_map<std::string, std::vector<std::string>>&
        knight_moves,
    std::unordered_map<std::string, std::vector<std::vector<int64_t>>>& next_dp,
    size_t seq)
{
  for (const std::string& letter : letter_layout()) {
    // std::cout << " --- Debug --- " << '\n';
    for (size_t vowel = 0; vowel <= max_vowels; ++vowel) {
      // Letter: A, Seq: s, Vowel: v, Count: c
      // Is it possible to be at A at s-1 with v vowels?
      const int64_t count = dp_array.at(letter)[seq - 1][vowel];
      // std::cout << "Letter: " << letter << ", Seq: " << seq - 1
      //           << ", Vowel: " << vowel << ", Count: " << count << '\n';
      if (count == 0) {
        continue;
      }
      // For every possible knight move from letter, calc the the new vowel
      for (const std::string& dest : knight_moves.at(letter)) {
        const size_t new_vowel = vowel + (is_vowel(dest) ? 1 : 0);
        // Constraint violated continue
        if (new_vowel > max_vowels) {
          continue;
        }
        next_dp[dest][seq][new_vowel] += count;
      }
    }
  }
}

// Compute the total number of valid sequences
auto aggregate_dp(
    std::unordered_map<std::string, std::vector<std::vector<int64_t>>> dp_array)
    -> int64_t
{
  int64_t total = 0;
  // For every letter total the count of vowels
  for (const std::string& letter : letter_layout()) {
    for (size_t vowel = 0; vowel <= max_vowels; ++vowel) {
      total += dp_array[letter][sequence_length - 1][vowel];
    }
  }
  return total;
}

}  // namespace

// Pre compute all knight moves
auto get_knight_moves()
    -> std::unordered_map<std::string, std::vector<std::string>>
{
  auto letter_coords = get_letter_coords();

  auto coords_letter = invert_letter_layout(letter_coords);
  std::unordered_map<std::string, std::vector<std::string>> possible_moves;
  // Define possible moves of a Knight i.e., 1 is down, -1 is up, 2 is right, -2
  // is left
  const std::vector<std::pair<int, int>> directions = {
      {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

  for (const auto& [letter, coords] : letter_coords) {
    const int coord_x = coords.first;
    const int coord_y = coords.second;
    // Calculate possible knight moves
    for (const auto& direction : directions) {
      const int dir_x = coord_x + direction.first;
      const int dir_y = coord_y + direction.second;
      // Check if the outter map contains dir_x if dir_x exists we check dir_y
      if (coords_letter.count(dir_x) != 0U
          && coords_letter.at(dir_x).count(dir_y) != 0U)
      {
        // if both x and y exist push the intersection of the two (key layout)
        possible_moves[letter].push_back(coords_letter.at(dir_x).at(dir_y));
      }
    }
  }
  return possible_moves;
}

auto count_knight_sequences() -> int64_t
{
  auto knight_moves = get_knight_moves();
  // Map of letter to <seq_position, vowel_count> init to 0
  std::unordered_map<std::string, std::vector<std::vector<int64_t>>> dp_array =
      init_vowels_dp();

  // Loop all of letter_layout and set vowel to 1 if vowel
  for (const std::string& letter : letter_layout()) {
    const size_t vowel = is_vowel(letter) ? 1 : 0;
    dp_array[letter][0][vowel] = 1;
  }

  for (size_t seq = 1; seq < sequence_length; ++seq) {
    std::unordered_map<std::string, std::vector<std::vector<int64_t>>> next_dp =
        init_vowels_dp();
    dp_step(dp_array, knight_moves, next_dp, seq);
    dp_array = next_dp;
  }

  const int64_t total = aggregate_dp(dp_array);
  return total;
}
