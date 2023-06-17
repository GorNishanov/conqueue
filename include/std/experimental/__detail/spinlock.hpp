// Copyright (c) 2023 Gor Nishanov
// Licensed under MIT license. See LICENSE.txt for details.

#ifndef _STD_EXPERIMENTAL_CONQUEUE_SPINLOCK
#define _STD_EXPERIMENTAL_CONQUEUE_SPINLOCK

#include <atomic>

namespace std::experimental::__detail {

// Thank you, bing chat, once again.

class spinlock {
  std::atomic_bool lock_{}; // initially clear

public:
  void lock() {
    bool expected = false;
    // try to acquire the lock by setting the flag to true
    while (!lock_.compare_exchange_weak(expected, true)) {
      // if failed, wait for the flag to change or be notified
      lock_.wait(true); // wait while the flag is true
      expected = false; // reset expected value
    }
    // lock acquired
  }

  void unlock() {
    // release the lock by setting the flag to false
    lock_.store(false);
    // notify one thread waiting on the flag
    lock_.notify_one();
  }
};

} // namespace std::experimental::__detail

#endif // _STD_EXPERIMENTAL_CONQUEUE_SPINLOCK
