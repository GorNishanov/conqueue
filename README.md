# conqueue

[![clang-14](https://github.com/GorNishanov/conqueue/actions/workflows/ci.yml/badge.svg)](https://github.com/GorNishanov/conqueue/actions/workflows/ci.yml)

experimenting with adding async_pop / push to https://wg21.link/P0260R5

Inspired by

- https://wg21.link/P0260R5 A proposal to add a concurrent queue
  to the standard library and
- https://wg21.link/p1958 A proposal to add a concurrent queue to the standard library

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
  std::optional<T> try_pop();
  std::optional<T> try_pop(std::error_code &ec);

  // For expediency push(T&&) and push(T const&) were collapsed into
  // push(T). Real implementation can split them up.

  push_sender async_push(T x) noexcept(is_nothrow_move_constructible_v<T>);
  void push(T x);
  bool push(T x, error_code &ec);
  bool try_push(T x);
  bool try_push(T x, error_code &ec);
};
```
