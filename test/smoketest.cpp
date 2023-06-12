#include <std/experimental/conqueue>

#include <catch2/catch_test_macros.hpp>

#include <exec/async_scope.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/task.hpp>

#include <stdexec/execution.hpp>

using namespace std;
using namespace std::experimental;

using stdexec::on;

TEST_CASE("smoketest", "[smoketest]") {
  buffer_queue<int> q(2);
  q.push(1);
  q.push(2);

  REQUIRE(q.pop() == 1);
  REQUIRE(q.pop() == 2);
}

#if 0
exec::task<void> coro_push(buffer_queue<int> &q) {
  co_await q.async_push(3);
  co_await q.async_push(4);
}

TEST_CASE("coro_push") {
  exec::static_thread_pool pool(1);
  exec::async_scope scope;
  buffer_queue<int> q(2);
  q.push(1);
  q.push(2);

  scope.spawn(stdexec::on(pool.get_scheduler(), coro_push(q)));

  REQUIRE(q.pop() == 1);
  REQUIRE(q.pop() == 2);
  REQUIRE(q.pop() == 3);
  REQUIRE(q.pop() == 4);

  stdexec::sync_wait(scope.on_empty());
}

exec::task<void> coro_pop(buffer_queue<int> &q) {
  REQUIRE(co_await q.async_pop() == 1);
  REQUIRE(co_await q.async_pop() == 2);
  REQUIRE(co_await q.async_pop() == 3);
  REQUIRE(co_await q.async_pop() == 4);
}

TEST_CASE("coro_pop") {
  exec::static_thread_pool pool(1);
  exec::async_scope scope;
  buffer_queue<int> q(2);

  scope.spawn(on(pool.get_scheduler(), coro_pop(q)));

  q.push(1);
  q.push(2);
  q.push(3);
  q.push(4);

  stdexec::sync_wait(scope.on_empty());
}
#endif
