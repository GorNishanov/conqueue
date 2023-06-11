#include <catch2/catch_test_macros.hpp>

#include <std/experimental/conqueue>

using namespace std;
using namespace std::experimental;

TEST_CASE("smoketest", "[smoketest]") {
  buffer_queue<int> q(10);
  REQUIRE(1 == 1);
}
