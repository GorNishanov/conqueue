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
  REQUIRE(list.front() == nullptr);
  REQUIRE(list.back() == nullptr);

  Item a{1}, b{2}, c{3};
  list.push_front(&a);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &a);
  REQUIRE(list.back() == &a);

  list.push_front(&b);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &b);
  REQUIRE(list.back() == &a);

  list.push_back(&c);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &b);
  REQUIRE(list.back() == &c);

  list.remove(&a);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &b);
  REQUIRE(list.back() == &c);

  list.remove(&b);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &c);
  REQUIRE(list.back() == &c);

  list.remove(&c);

  REQUIRE(list.empty());
}
