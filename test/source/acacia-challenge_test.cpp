#include <cstdint>

#include <boost/range/algorithm/find.hpp>
#include <catch2/catch_test_macros.hpp>

#include "lib.hpp"

// Check valid knight moves

TEST_CASE("B knight moves include I, M and K", "[knight][move]")
{
  auto moves_from_b = get_knight_moves()["B"];

  const bool has_i =
      boost::range::find(moves_from_b, "I") != moves_from_b.end();
  const bool has_m =
      boost::range::find(moves_from_b, "M") != moves_from_b.end();
  const bool has_k =
      boost::range::find(moves_from_b, "K") != moves_from_b.end();

  REQUIRE(has_i);
  REQUIRE(has_m);
  REQUIRE(has_k);
}

TEST_CASE("O knight moves include D and 2", "[knight][move]")
{
  auto moves_from_o = get_knight_moves()["O"];

  const bool has_d =
      boost::range::find(moves_from_o, "D") != moves_from_o.end();
  const bool has_2 =
      boost::range::find(moves_from_o, "2") != moves_from_o.end();
  const bool has_h =
      boost::range::find(moves_from_o, "H") != moves_from_o.end();

  REQUIRE(has_d);
  REQUIRE(has_2);
  REQUIRE(has_h);
}

TEST_CASE("H knight moves include A, E, K, O, 1, 3", "[knight][move]")
{
  auto moves_from_h = get_knight_moves()["H"];

  const bool has_a =
      boost::range::find(moves_from_h, "A") != moves_from_h.end();
  const bool has_e =
      boost::range::find(moves_from_h, "E") != moves_from_h.end();
  const bool has_k =
      boost::range::find(moves_from_h, "K") != moves_from_h.end();
  const bool has_o =
      boost::range::find(moves_from_h, "O") != moves_from_h.end();
  const bool has_1 =
      boost::range::find(moves_from_h, "1") != moves_from_h.end();
  const bool has_3 =
      boost::range::find(moves_from_h, "3") != moves_from_h.end();

  REQUIRE(has_a);
  REQUIRE(has_e);
  REQUIRE(has_k);
  REQUIRE(has_o);
  REQUIRE(has_1);
  REQUIRE(has_3);
}

TEST_CASE(
    "count_knight_sequences returns the correct total for a 10-move sequence "
    "with at most 2 vowels allowed",
    "[knight][sequence]")
{
  const int64_t total = count_knight_sequences();
  REQUIRE(total == 1013398);
}
