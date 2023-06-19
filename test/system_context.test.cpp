#include "std/experimental/system_context"
#include "exec/async_scope.hpp"
#include "stdexec/execution.hpp"
#include <catch2/catch_test_macros.hpp>
#include <exec/task.hpp>

#include <stdio.h>

#include <chrono>
using namespace std::literals;

// dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,0)

// void dispatch_async_f(dispatch_queue_t queue, void *context, void
// (*function)(void *));

// dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,0),
// ^{
//         // background operation
// });

// bulk
// void dispatch_apply_f(size_t iterations, dispatch_queue_t queue, void
// *context, void (*function)(void *, size_t));

// void
// dispatch_after_f(dispatch_time_t when, dispatch_queue_t queue, void *context,
// void (*function)(void *));

// DISPATCH_TIME_NOW, DISPATCH_TIME_FOREVER

// NAME
//      dispatch_time, dispatch_walltime – Calculate temporal milestones

// SYNOPSIS
//      #include <dispatch/dispatch.h>

//      static const dispatch_time_t DISPATCH_TIME_NOW = 0ull;
//      static const dispatch_time_t DISPATCH_WALLTIME_NOW = ~1ull;
//      static const dispatch_time_t DISPATCH_TIME_FOREVER = ~0ull;

//      dispatch_time_t
//      dispatch_time(dispatch_time_t base, int64_t offset);

//      dispatch_time_t
//      dispatch_walltime(struct timespec *base, int64_t offset);

// DESCRIPTION
//      The dispatch_time() and dispatch_walltime() functions provide a simple
//      mechanism for expressing temporal milestones for use with dispatch
//      functions that need timeouts or operate on a schedule.

//      The dispatch_time_t type is a semi-opaque integer, with only the special
//      values DISPATCH_TIME_NOW, DISPATCH_WALLTIME_NOW and
//      DISPATCH_TIME_FOREVER being externally defined.  All other values are
//      represented using an internal format that is not safe for integer
//      arithmetic or comparison.  The internal format is subject to change.

//      The dispatch_time() function returns a milestone relative to an existing
//      milestone after adding offset nanoseconds.  If the base parameter maps
//      internally to a wall clock or is DISPATCH_WALLTIME_NOW, then the
//      returned value is relative to the wall clock.  Otherwise, if base is
//      DISPATCH_TIME_NOW, then the current time of the default host clock is
//      used.  On Apple platforms, the value of the default host clock is
//      obtained from mach_absolute_time().

//      The dispatch_walltime() function is useful for creating a milestone
//      relative to a fixed point in time using the wall clock, as specified by
//      the optional base parameter.  If base is NULL, then the current time of
//      the wall clock is used. dispatch_walltime(NULL, offset) is equivalent to
//      dispatch_time(DISPATCH_WALLTIME_NOW, offset).

// EDGE CONDITIONS
//      The dispatch_time() and dispatch_walltime() functions detect overflow
//      and underflow conditions when applying the offset parameter.

//      Overflow causes DISPATCH_TIME_FOREVER to be returned.  When base is
//      DISPATCH_TIME_FOREVER, then the offset parameter is ignored.

//      Underflow causes the smallest representable value to be returned for a
//      given clock.

// EXAMPLES
//      Create a milestone two seconds in the future, relative to the default
//      clock:

//            milestone = dispatch_time(DISPATCH_TIME_NOW, 2 * NSEC_PER_SEC);

//      Create a milestone two seconds in the future, in wall clock time:

//            milestone = dispatch_time(DISPATCH_WALLTIME_NOW, 2 *
//            NSEC_PER_SEC);

//      Create a milestone for use as an infinite timeout:

//            milestone = DISPATCH_TIME_FOREVER;

//      Create a milestone on Tuesday, January 19, 2038:

//            struct timespec ts;
//            ts.tv_sec = 0x7FFFFFFF;
//            ts.tv_nsec = 0;
//            milestone = dispatch_walltime(&ts, 0);

//      Use a negative delta to create a milestone an hour before the one above:

//            milestone = dispatch_walltime(&ts, -60 * 60 * NSEC_PER_SEC);

// RETURN VALUE
//      These functions return an abstract value for use with dispatch_after(),
//      dispatch_group_wait(), dispatch_semaphore_wait(), or
//      dispatch_source_set_timer().

// SEE ALSO
//      dispatch(3), dispatch_after(3), dispatch_group_create(3),
//      dispatch_semaphore_create(3)

exec::task<void> coro(std::experimental::system_context& ctx, int val) {
  printf("coro%d: started\n", val);
  co_await ctx.get_scheduler().schedule_after(1000s);
  printf("coro%d: done\n", val);
}

TEST_CASE("system_context: hello") {
  puts("Hello");
  std::experimental::system_context ctx;
  auto sched = ctx.get_scheduler();
  exec::async_scope scope;

  for (int i = 1; i < 10; ++i)
    scope.spawn(stdexec::on(sched, coro(ctx, i)));

  std::this_thread::sleep_for(50ms);
  scope.request_stop();

  stdexec::sync_wait(scope.on_empty());
  puts("World");
}
