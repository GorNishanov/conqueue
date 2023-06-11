#include <catch2/catch_test_macros.hpp>

#include <std/experimental/conqueue>

using namespace std;
using namespace std::experimental;

TEST_CASE("smoketest", "[smoketest]") {
  buffer_queue<int> q(10);
  q.push(1);
  q.push(2);

  REQUIRE(q.pop() == 1);
  REQUIRE(q.pop() == 2);
}
