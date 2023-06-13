# conqueue

[![clang-14](https://github.com/GorNishanov/conqueue/actions/workflows/ci.yml/badge.svg)](https://github.com/GorNishanov/conqueue/actions/workflows/ci.yml)

Experimental implementation of concurrent buffer queue with adding sender/receiver based async_pop / async_push to https://wg21.link/p1958r0. Using reference implementation of sender/receiver from https://github.com/NVIDIA/stdexec.

Inspired by

- https://wg21.link/p0260r5 A proposal to add a concurrent queue
  to the standard library and
- https://wg21.link/p1958r0 C++ Concurrent Buffer Queue
- https://wg21.link/p2882r0 An Event Model for C++ Executors

```c++
template <typename T> class buffer_queue {
public:
  typedef T value_type;
  explicit buffer_queue(size_t max_elems);
  template <typename Iter>
  buffer_queue(size_t max_elems, Iter first, Iter last);
  ~buffer_queue() noexcept;
  void close() noexcept;

  pop_sender async_pop() noexcept;
  T pop();
  std::optional<T> pop(std::error_code &ec);
  std::optional<T> try_pop(std::error_code &ec);

  // For expediency push(T&&) and push(T const&) were collapsed into
  // push(T). Real implementation can split them up.

  push_sender async_push(T x) noexcept(is_nothrow_move_constructible_v<T>);
  void push(T x);
  bool push(T x, error_code &ec);
  bool try_push(T x, error_code &ec);
};
```
