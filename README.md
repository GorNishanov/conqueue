# conqueue

[![clang-14](https://github.com/GorNishanov/conqueue/actions/workflows/ci.yml/badge.svg)](https://github.com/GorNishanov/conqueue/actions/workflows/ci.yml)

Supporting material for
paper: [d2912r0: Concurrent queues and sender/receivers](d2912r0.md)

Experimental implementation of concurrent buffer queue with adding sender/receiver based async_pop / async_push to https://wg21.link/p1958r0. Using reference implementation of sender/receiver from https://github.com/NVIDIA/stdexec.

Inspired by

- https://wg21.link/p0260r5 A proposal to add a concurrent queue
  to the standard library
- https://wg21.link/p1958r0 C++ Concurrent Buffer Queue
- https://wg21.link/p2882r0 An Event Model for C++ Executors

```c++
template <typename T, typename Alloc = std::allocator<T>> class buffer_queue {
public:
  using value_type = T;

  explicit buffer_queue(size_t max_elems, Alloc alloc = Alloc());
  ~buffer_queue() noexcept;

  // observers
  bool is_closed() noexcept;
  size_t capacity() const noexcept;

  // modifiers
  void close() noexcept;

  pop_sender async_pop() noexcept;
  T pop();
  std::optional<T> pop(std::error_code& ec);
  std::optional<T> try_pop(std::error_code& ec);

  push_sender async_push(T x) noexcept(is_nothrow_move_constructible_v<T>);
  void push(const T& x);
  bool push(const T& x, error_code& ec); // used to be wait_push
  bool try_push(const T& x, error_code& ec);

  void push(T&& x);
  bool push(T&& x, error_code& ec); // used to be wait_push
  bool try_push(T&& x, error_code& ec);
};
```
