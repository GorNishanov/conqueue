// Copyright (c) 2023 Gor Nishanov
// Licensed under MIT license. See LICENSE.txt for details.

#ifndef _STD_EXPERIMENTAL_CONQUEUE_RING_BUFFER
#define _STD_EXPERIMENTAL_CONQUEUE_RING_BUFFER

#include <algorithm>
#include <cassert>
#include <memory>

namespace std::experimental::__detail {

// Thank you, bing chat, once again.

template <typename T, typename Alloc = std::allocator<T>> class ring_buffer {
  using alloc_traits = allocator_traits<Alloc>;

  size_t index_of(size_t i) const { return (head_ + i) % capacity_; }

public:
  explicit ring_buffer(size_t capacity, const Alloc& alloc = Alloc())
      : alloc_(alloc), capacity_(capacity) {
    if (capacity != 0)
      buffer_ = alloc_traits::allocate(alloc_, capacity_);
  }

  explicit ring_buffer(std::initializer_list<T> init, size_t capacity = 0,
                       const Alloc& alloc = Alloc())
      : ring_buffer(std::max(capacity, init.size()), alloc),
        size_(init.size()) {
    std::uninitialized_fill(buffer_, buffer_ + size_, init.begin());
  }

  template <typename InputIterator>
  ring_buffer(InputIterator first, InputIterator last, size_t capacity,
              const Alloc& alloc = Alloc())
      : ring_buffer(capacity, alloc) {
    try {
      for (; first != last; ++first)
        push_back(*first);
    } catch (...) {
      if (buffer_)
        alloc_traits::deallocate(alloc, buffer_, capacity_);
      throw;
    }
  }

  ~ring_buffer() {
    for (size_t i = 0; i < size_; i++)
      alloc_traits::destroy(alloc_, buffer_ + index_of(i));

    if (buffer_)
      alloc_traits::deallocate(alloc_, buffer_, capacity_);
  }

  bool full() const noexcept { return size_ == capacity_; }
  bool empty() const noexcept { return size_ == 0; }
  size_t capacity() const noexcept { return capacity_; }
  size_t size() const noexcept { return size_; }

  void push_back(const T& value) {
    assert(not full());
    alloc_traits::construct(alloc_, buffer_ + index_of(size_), value);
    size_++;
  }

  void push_back(T&& value) {
    assert(not full());
    alloc_traits::construct(alloc_, buffer_ + index_of(size_),
                            std::move(value));
    size_++;
  }

  T pop_front() {
    assert(not empty());
    T& ref = buffer_[head_];

    // In order to avoid bad elements that cannot be moved out of the queue
    // we remove the element first and then return it by moving it into result.
    head_ = (head_ + 1) % capacity_;
    size_--;

    try {
      T result{std::move(ref)};
      alloc_traits::destroy(alloc_, std::addressof(ref));
      return result;
    } catch (...) {
      // If the move constructor throws, destroy the element nonetheless.
      alloc_traits::destroy(alloc_, std::addressof(ref));
      throw;
    }
  }

private:
  [[no_unique_address]] Alloc alloc_; // the allocator
  size_t capacity_{}; // maximum number of elements in the buffer
  size_t size_{};     // current number of elements in the buffer
  size_t head_{};     // index of the first element in the buffer
  T* buffer_{};       // pointer to the allocated memory for the buffer
};

} // namespace std::experimental::__detail

#endif // _STD_EXPERIMENTAL_CONQUEUE_INTRUSIVE_LIST
