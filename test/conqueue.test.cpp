#define STDEX_CONQUEUE_ENABLE_TRACING
#include <std/experimental/conqueue>

#include <catch2/catch_test_macros.hpp>

#include <exec/async_scope.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/task.hpp>

#include <stdexec/execution.hpp>

#include <chrono>
#include <system_error>
#include <thread>

using namespace std;
using namespace std::experimental;
using namespace std::literals;

using stdexec::on;

TEST_CASE("conqueue: smoketest") {
  buffer_queue<int> q(2);
  REQUIRE_FALSE(q.is_closed());
  q.push(1);
  q.push(2);
  std::error_code ec;
  REQUIRE_FALSE(q.try_push(3, ec));
  REQUIRE(ec == conqueue_errc::full);

  REQUIRE(q.pop() == 1);
  REQUIRE(q.pop() == 2);
}

TEST_CASE("conqueue: initially closed test") {
  buffer_queue<int> q(2);
  REQUIRE_FALSE(q.is_closed());
  q.close();
  REQUIRE(q.is_closed());
  SECTION("push") {
    error_code ec;
    REQUIRE_FALSE(q.push(1, ec));
    REQUIRE(ec == conqueue_errc::closed);
    REQUIRE_THROWS_AS(q.push(1), conqueue_error);
  }
  SECTION("pop") {
    error_code ec;
    REQUIRE_FALSE(q.pop(ec));
    REQUIRE(ec == conqueue_errc::closed);
    REQUIRE_THROWS_AS(q.pop(), conqueue_error);
  }
}

TEST_CASE("conqueue: pull from closed") {
  buffer_queue<int> q(2);
  q.push(1);
  q.push(2);
  q.close();
  REQUIRE(q.pop() == 1);
  REQUIRE(q.pop() == 2);
  error_code ec;
  REQUIRE_FALSE(q.pop(ec));
  REQUIRE(ec == conqueue_errc::closed);
  REQUIRE_THROWS_AS(q.pop(), conqueue_error);
}

TEST_CASE("conqueue: blocking pull then closed") {
  buffer_queue<int> q(2);
  thread t([&q] {
    this_thread::sleep_for(10ms);
    q.push(1);
    q.close();
  });

  // Make sure that the queue is empty.
  std::error_code ec;
  REQUIRE_FALSE(q.try_pop(ec));
  REQUIRE(ec == conqueue_errc::empty);

  REQUIRE(q.pop() == 1);
  REQUIRE_THROWS_AS(q.pop(), conqueue_error);
  t.join();
}

exec::task<void> coro_push(buffer_queue<int>& q, int from = 3, int to = 4) {
  for (; from <= to; ++from)
    co_await q.async_push(from);
}

TEST_CASE("conqueue: coro_push") {
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

exec::task<void> coro_pop(buffer_queue<int>& q) {
  REQUIRE(co_await q.async_pop() == 1);
  REQUIRE(co_await q.async_pop() == 2);
  REQUIRE(co_await q.async_pop() == 3);
  REQUIRE(co_await q.async_pop() == 4);
}

TEST_CASE("conqueue: coro_pop") {
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

TEST_CASE("conqueue: coro_pop rendezvous") {
  exec::static_thread_pool pool(1);
  exec::async_scope scope;
  buffer_queue<int> q(0);

  scope.spawn(on(pool.get_scheduler(), coro_pop(q)));

  q.push(1);
  q.push(2);
  q.push(3);
  q.push(4);

  stdexec::sync_wait(scope.on_empty());
}

TEST_CASE("conqueue: coro_push rendezvous") {
  exec::static_thread_pool pool(1);
  exec::async_scope scope;
  buffer_queue<int> q(0);

  scope.spawn(on(pool.get_scheduler(), coro_push(q, 1, 4)));

  REQUIRE(q.pop() == 1);
  REQUIRE(q.pop() == 2);
  REQUIRE(q.pop() == 3);
  REQUIRE(q.pop() == 4);

  stdexec::sync_wait(scope.on_empty());
}

exec::task<void> coro_stuck_pop(buffer_queue<int>& q) {
  co_await q.async_pop();
}

TEST_CASE("conqueue: cancellation async_pop") {
  exec::static_thread_pool pool(1);
  auto sched = pool.get_scheduler();
  exec::async_scope scope;
  buffer_queue<int> q(2);

  scope.spawn(on(sched, coro_stuck_pop(q)));
  std::this_thread::sleep_for(10ms);
  scope.request_stop();
  stdexec::sync_wait(scope.on_empty());
}

TEST_CASE("conqueue: cancellation async_push") {
  exec::static_thread_pool pool(1);
  auto sched = pool.get_scheduler();
  exec::async_scope scope;
  buffer_queue<int> q(0);

  scope.spawn(on(sched, coro_push(q)));
  std::this_thread::sleep_for(10ms);
  scope.request_stop();
  stdexec::sync_wait(scope.on_empty());
}
