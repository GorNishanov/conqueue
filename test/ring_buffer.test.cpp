#include "std/experimental/__detail/ring_buffer.hpp"
#include <catch2/catch_test_macros.hpp>
#include <utility>

using namespace std::experimental::__detail;

int default_ctor_count = 0;
int value_ctor_count = 0;
int move_ctor_count = 0;
int copy_ctor_count = 0;
int dtor_count = 0;

void reset_counts() {
  copy_ctor_count = 0;
  default_ctor_count = 0;
  value_ctor_count = 0;
  move_ctor_count = 0;
  dtor_count = 0;
}

void verify_counts(int default_ctors, int value_ctors, int move_ctors,
                   int copy_ctors, int dtors) {
  REQUIRE(copy_ctor_count == copy_ctors);
  REQUIRE(default_ctor_count == default_ctors);
  REQUIRE(value_ctor_count == value_ctors);
  REQUIRE(move_ctor_count == move_ctors);
  REQUIRE(dtor_count == dtors);
}

struct Item {
  Item() { ++default_ctor_count; }
  explicit Item(int v) : val{v} { ++value_ctor_count; }
  Item(const Item& rhs) : val(rhs.val) { ++copy_ctor_count; }
  Item(Item&& rhs) : val(std::exchange(rhs.val, 0)) { ++move_ctor_count; }
  ~Item() { ++dtor_count; }
  int val{};
};

TEST_CASE("ring_buffer: zero capacity") {
  reset_counts();
  ring_buffer<Item> rb{0};
  REQUIRE(rb.empty());
  REQUIRE(rb.full());
  REQUIRE(rb.capacity() == 0);
  REQUIRE(rb.size() == 0);
  verify_counts(0, 0, 0, 0, 0);
}

TEST_CASE("ring_buffer: capacity 1") {
  reset_counts();
  {
    ring_buffer<Item> rb{1};
    REQUIRE(rb.empty());
    REQUIRE_FALSE(rb.full());
    REQUIRE(rb.capacity() == 1);
    REQUIRE(rb.size() == 0);
    verify_counts(0, 0, 0, 0, 0);
    rb.push_back(Item{1});
    REQUIRE_FALSE(rb.empty());
    REQUIRE(rb.full());
    REQUIRE(rb.capacity() == 1);
    REQUIRE(rb.size() == 1);
    verify_counts(0, 1, 1, 0, 1);
    Item v = rb.pop_front();
    REQUIRE(v.val == 1);
    REQUIRE(rb.empty());
    REQUIRE_FALSE(rb.full());
    REQUIRE(rb.capacity() == 1);
    REQUIRE(rb.size() == 0);
    verify_counts(0, 1, 2, 0, 2);
    rb.push_back(Item{2});
    REQUIRE_FALSE(rb.empty());
    REQUIRE(rb.full());
    REQUIRE(rb.capacity() == 1);
    REQUIRE(rb.size() == 1);
    verify_counts(0, 2, 3, 0, 3);
    Item w = rb.pop_front();
    REQUIRE(w.val == 2);
    REQUIRE(rb.empty());
    REQUIRE_FALSE(rb.full());
    REQUIRE(rb.capacity() == 1);
    REQUIRE(rb.size() == 0);
    verify_counts(0, 2, 4, 0, 4);
  }
  verify_counts(0, 2, 4, 0, 6);
}

TEST_CASE("ring_buffer larger capacity") {
  ring_buffer<Item> rb{3};
  for (int i = 0; i < 20; i += 2) {
    rb.push_back(Item{i});
    rb.push_back(Item{i + 1});
    REQUIRE(rb.pop_front().val == i);
    REQUIRE(rb.pop_front().val == i + 1);
  }
}