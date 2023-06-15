#include <exec/async_scope.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/task.hpp>
#include <iostream>
#include <stdexec/execution.hpp>

exec::task<void> coro() {
  auto stop_token = co_await stdexec::get_stop_token();
  std::cout << "stop possible: " << stop_token.stop_possible() << "\n";
}

int main() {
  exec::static_thread_pool pool(1);
  exec::async_scope scope;
  scope.spawn(stdexec::on(pool.get_scheduler(), coro()));
  stdexec::sync_wait(scope.on_empty());
}
