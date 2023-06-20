# as_expected sender adapter

[std::execution](https://wg21.link/p2300) paper offers a convenience sender adapters, 
`stopped_as_optional` and `stopped_as_error<E>` that allow to convert stop propagation
into a value channel (as `nullopt`) or through an error channel as error `E`.

We propose adding `as_expected` sender adapter that will convert errors of 
`std::exception_ptr`, `error_code` enums or `std::system_error` into
`std::expected<T, std::error_code>`.

## Use case: buffer_queue

buffer_queue offers a throwing and non-throwing version of synchronous APIs as follows:

```c++
void buffer_queue::push(T&&); // throws if queue is closed.
bool buffer_queue::push(T&&, std::error_code& ec); // returns false and set ec if queue is closed.

q.push(5); // throws on closed;

std::error_code ec;
if (!q.push(ec))
    std::println("Failed to push. Error {}", ec);
```

For async versions, I do not want to duplicate the throwing and non-throwing
versions, instead, I would like to have only a single `async_push` API
that by default is throwing and can be converted into non-throwing
via `std::execution::as_expected` adapter.

```c++
co_await q.async_push(5); // will throw if q is closed

auto result1 = co_await as_expected(q.async_push(5));
auto result2 = co_await (q.async_push(5) | as_expected());
// or even perhaps (as a convenience member on a sender:
auto result3 = co_await q.async_push(5).as_expected();
```


