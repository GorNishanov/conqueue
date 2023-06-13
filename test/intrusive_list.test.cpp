#include "std/experimental/__detail/intrusive_list.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace std::experimental::__detail;

struct Item {
  int val{};
  Item* next{};
  Item* prev{};
};

TEST_CASE("intrusive_list: smoketest") {
  intrusive_list<&Item::next, &Item::prev> list;
  REQUIRE(list.empty());
}
