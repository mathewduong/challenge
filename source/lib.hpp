#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

// Return all knight moves for each key
auto get_knight_moves()
    -> std::unordered_map<std::string, std::vector<std::string>>;

// Count all valid knight-move sequences
auto count_knight_sequences() -> int64_t;
