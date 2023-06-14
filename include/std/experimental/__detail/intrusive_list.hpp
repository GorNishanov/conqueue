// Copyright (c) 2023 Gor Nishanov
// Licensed under MIT license. See LICENSE.txt for details.

#ifndef _STD_EXPERIMENTAL_CONQUEUE_INTRUSIVE_LIST
#define _STD_EXPERIMENTAL_CONQUEUE_INTRUSIVE_LIST

namespace std::experimental::__detail {

// Thank you bing chat. You did good. Only two bugs needed to be fixed.

// A class that represents an intrusive doubly-linked list
template <auto _Next, auto _Prev> class intrusive_list;

// A partial specialization for object pointers that have next and prev pointers
template <class _Item, _Item* _Item::*_Next, _Item* _Item::*_Prev>
class intrusive_list<_Next, _Prev> {
public:
  bool empty() const { return head_ == nullptr; }

  // Insert an object at the front of the list
  void push_front(_Item* obj) {
    // Link the object with the current head
    obj->*_Next = head_;
    obj->*_Prev = nullptr;

    // Update the head pointer
    if (head_)
      head_->*_Prev = obj;
    head_ = obj;

    // Update the tail pointer if the list was empty
    if (!tail_)
      tail_ = obj;
  }

  // Insert an object at the back of the list
  void push_back(_Item* obj) {
    // Link the object with the current tail
    obj->*_Prev = tail_;
    obj->*_Next = nullptr;

    // Update the tail pointer
    if (tail_)
      tail_->*_Next = obj;
    tail_ = obj;

    // Update the head pointer if the list was empty
    if (!head_)
      head_ = obj;
  }

  // Remove an object from the front of the list and return it
  _Item* pop_front() {
    // Check if the list is empty
    if (!head_)
      return nullptr;

    // Get the object pointer from the head pointer
    _Item* obj = head_;

    // Update the head pointer
    head_ = head_->*_Next;

    // Update the prev pointer of the next object to point to nullptr
    if (head_)
      head_->*_Prev = nullptr;
    else
      // Update the tail pointer if the list becomes empty
      tail_ = nullptr;

    // Unlink the object from the list
    obj->*_Prev = nullptr;
    obj->*_Next = nullptr;

    // Return the object pointer
    return obj;
  }

  // Remove an object from the back of the list and return it
  _Item* pop_back() {
    // Check if the list is empty
    if (!tail_)
      return nullptr;

    // Get the object pointer from the tail pointer
    _Item* obj = tail_;

    // Update the tail pointer
    tail_ = tail_->*_Prev;

    // Update the next pointer of the previous object to point to nullptr
    if (tail_)
      tail_->*_Next = nullptr;
    else
      // Update the head pointer if the list becomes empty
      head_ = nullptr;

    // Unlink this object from the list
    obj->*_Prev = nullptr;
    obj->*_Next = nullptr;

    // Return the object pointer
    return obj;
  }

  // Remove a given object from anywhere in the list
  // Precondition: obj is in the list.
  void remove(_Item* obj) {
    // Get pointers to previous and next objects
    _Item* prev = obj->*_Prev;
    _Item* next = obj->*_Next;

    // Unlink this object from its neighbors
    if (prev)
      prev->*_Next = next;
    if (next)
      next->*_Prev = prev;

    // Update head and tail pointers if needed
    if (obj == head_)
      head_ = next;
    if (obj == tail_)
      tail_ = prev;

    // Unlink this object from the list
    obj->*_Prev = nullptr;
    obj->*_Next = nullptr;
  }

  // Remove a given object from anywhere in the list if it is in the list.
  // Precondition: obj is in the list or was already removed from the list.
  bool try_remove(_Item* obj) {
    if (obj->*_Prev == nullptr && obj->*_Next == nullptr && front() != obj)
      return false;

    remove(obj);
    return true;
  }

  // Get the head pointer of the list
  _Item* front() const { return head_; }

  // Get the tail pointer of the list
  _Item* back() const { return tail_; }

private:
  // Pointers to the first and last objects of the list
  _Item* head_{};
  _Item* tail_{};
};
} // namespace std::experimental::__detail

#endif // _STD_EXPERIMENTAL_CONQUEUE_INTRUSIVE_LIST
