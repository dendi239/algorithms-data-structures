#include <catch2/catch_all.hpp>

#include "../algebra/modular.hpp"

namespace pure_env {

using Mint = Z<13>;

Mint operator""_z(unsigned long long value) {
  return Mint{static_cast<int64_t>(value)};
}

}  // namespace pure_env

TEST_CASE("Simple modular test") {
  using namespace pure_env;

  REQUIRE(2_z + 13_z == 15_z);
  REQUIRE(-1_z + 2_z == 1_z);
  REQUIRE(10_z + 20_z == 4_z);

  REQUIRE(2_z - 4_z == 11_z);
  REQUIRE(2_z - 2_z == 0_z);
  REQUIRE(2_z + 10_z == -1_z);

  REQUIRE(2 * 2_z == 4_z);
  REQUIRE(-2 * 2_z == 9_z);
  REQUIRE(3 * 5_z == 2_z);

  REQUIRE(1_z / 2_z == 7_z);
  REQUIRE(6_z / 3_z == 2_z);
  REQUIRE(6_z / 5_z == 9_z);
}

namespace dynamic_env {

using Mint = DynamicModular<int64_t>;

Mint operator""_z(unsigned long long value) {
  return Mint{static_cast<int64_t>(value)};
}

}  // namespace dynamic_env

TEST_CASE("Dynamic modulo test") {
  using namespace dynamic_env;
  Mint::ModuloType::value = 13;

  REQUIRE(2_z + 13_z == 15_z);
  REQUIRE(-1_z + 2_z == 1_z);
  REQUIRE(10_z + 20_z == 4_z);

  REQUIRE(2_z - 4_z == 11_z);
  REQUIRE(2_z - 2_z == 0_z);
  REQUIRE(2_z + 10_z == -1_z);

  REQUIRE(2 * 2_z == 4_z);
  REQUIRE(-2 * 2_z == 9_z);
  REQUIRE(3 * 5_z == 2_z);

  REQUIRE(1_z / 2_z == 7_z);
  REQUIRE(6_z / 3_z == 2_z);
  REQUIRE(6_z / 5_z == 9_z);
}
