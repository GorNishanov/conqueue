#include <std/experimental/conqueue>

#include <catch2/catch_test_macros.hpp>

#include <exec/async_scope.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/task.hpp>

using namespace std;
using namespace std::experimental;

TEST_CASE("smoketest", "[smoketest]") {
  buffer_queue<int> q(2);
  q.push(1);
  q.push(2);

  REQUIRE(q.pop() == 1);
  REQUIRE(q.pop() == 2);
}

exec::task<void> coro(buffer_queue<int> &q) {
  co_await q.async_push(3);
  co_await q.async_push(4);
}

TEST_CASE("coro", "[coro]") {
  exec::static_thread_pool pool(1);
  exec::async_scope scope;
  buffer_queue<int> q(2);
  q.push(1);
  q.push(2);

  REQUIRE(q.pop() == 1);
  REQUIRE(q.pop() == 2);
  REQUIRE(q.pop() == 3);
  REQUIRE(q.pop() == 4);
}
