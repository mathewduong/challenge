#include <cstdint>
#include <iostream>

#include "lib.hpp"

auto main() -> int
{
  const int64_t total = count_knight_sequences();
  std::cout << total << '\n';
  return 0;
}
