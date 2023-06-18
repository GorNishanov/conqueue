#include "std/experimental/__detail/intrusive_list.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace std::experimental::__detail;

struct Item {
  int val{};
  Item* next{};
  Item* prev{};
};

void test_invariant(intrusive_list<&Item::next, &Item::prev>& list) {
  if (list.empty()) {
    REQUIRE(list.front() == nullptr);
    REQUIRE(list.back() == nullptr);
  } else {
    REQUIRE(list.front() != nullptr);
    REQUIRE(list.back() != nullptr);
    if (list.front() == list.back()) {
      REQUIRE(list.front()->next == nullptr);
      REQUIRE(list.front()->prev == nullptr);
    } else {
      REQUIRE(list.front()->next != nullptr);
      REQUIRE(list.back()->prev != nullptr);
      REQUIRE(list.front()->prev == nullptr);
      REQUIRE(list.back()->next == nullptr);
    }
  }
}

TEST_CASE("intrusive_list: push and remove") {
  intrusive_list<&Item::next, &Item::prev> list;

  REQUIRE(list.empty());
  test_invariant(list);

  Item a{1}, b{2}, c{3};

  list.push_front(&a);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &a);
  REQUIRE(list.back() == &a);
  test_invariant(list);

  list.push_front(&b);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &b);
  REQUIRE(list.back() == &a);
  test_invariant(list);

  list.push_back(&c);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &b);
  REQUIRE(list.back() == &c);
  test_invariant(list);

  list.remove(&a);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &b);
  REQUIRE(list.back() == &c);
  test_invariant(list);

  list.remove(&b);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &c);
  REQUIRE(list.back() == &c);
  test_invariant(list);

  list.remove(&c);

  REQUIRE(list.empty());
  test_invariant(list);
}

TEST_CASE("intrusive_list: push and try_remove") {
  intrusive_list<&Item::next, &Item::prev> list;

  REQUIRE(list.empty());
  test_invariant(list);

  Item a{1}, b{2}, c{3};
  REQUIRE_FALSE(list.try_remove(&a));
  REQUIRE_FALSE(list.try_remove(&b));
  REQUIRE_FALSE(list.try_remove(&c));

  list.push_front(&a);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &a);
  REQUIRE(list.back() == &a);
  test_invariant(list);

  list.push_front(&b);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &b);
  REQUIRE(list.back() == &a);
  test_invariant(list);

  list.push_back(&c);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &b);
  REQUIRE(list.back() == &c);
  test_invariant(list);

  REQUIRE(list.try_remove(&a));
  REQUIRE_FALSE(list.try_remove(&a));

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &b);
  REQUIRE(list.back() == &c);
  test_invariant(list);

  REQUIRE(list.try_remove(&b));
  REQUIRE_FALSE(list.try_remove(&b));

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &c);
  REQUIRE(list.back() == &c);
  test_invariant(list);

  REQUIRE(list.try_remove(&c));
  REQUIRE_FALSE(list.try_remove(&c));

  REQUIRE(list.empty());
  test_invariant(list);
}

TEST_CASE("push and pop_back last") {
  intrusive_list<&Item::next, &Item::prev> list;
  Item a{1}, b{2}, c{3};

  REQUIRE(list.empty());

  list.push_back(&a);
  test_invariant(list);

  list.push_back(&b);
  test_invariant(list);

  list.push_back(&c);
  test_invariant(list);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &a);
  REQUIRE(list.back() == &c);
  test_invariant(list);

  REQUIRE(list.try_pop_front() == &a);
  REQUIRE(list.front() == &b);
  REQUIRE(list.back() == &c);
  REQUIRE_FALSE(list.empty());
  test_invariant(list);

  REQUIRE(list.try_pop_back() == &c);
  REQUIRE(list.front() == &b);
  REQUIRE(list.back() == &b);
  REQUIRE_FALSE(list.empty());
  test_invariant(list);

  REQUIRE(list.try_pop_back() == &b);
  REQUIRE(list.empty());
  test_invariant(list);
}

TEST_CASE("push and pop_front last") {
  intrusive_list<&Item::next, &Item::prev> list;
  Item a{1}, b{2}, c{3};

  REQUIRE(list.empty());

  list.push_back(&a);
  list.push_back(&b);
  list.push_back(&c);

  REQUIRE_FALSE(list.empty());
  REQUIRE(list.front() == &a);
  REQUIRE(list.back() == &c);
  test_invariant(list);

  REQUIRE(list.try_pop_front() == &a);
  REQUIRE(list.front() == &b);
  REQUIRE(list.back() == &c);
  REQUIRE_FALSE(list.empty());
  test_invariant(list);

  REQUIRE(list.try_pop_back() == &c);
  REQUIRE(list.front() == &b);
  REQUIRE(list.back() == &b);
  REQUIRE_FALSE(list.empty());
  test_invariant(list);

  REQUIRE(list.try_pop_front() == &b);
  REQUIRE(list.empty());
  test_invariant(list);
}
