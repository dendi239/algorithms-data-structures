//
// Copyright (c) 2020 Denys Smirnov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

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
