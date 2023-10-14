| Document Number: | d2927r0            |
| ---------------- | ------------------ |
| Date:            | 2023-06-16         |
| Target:          | EWG, LEWG, LEWGI   |
| Reply to:        | gorn@microsoft.com |

# Inspecting exception_ptr

## Abstract 

Provide facility to observe exceptions stored in `std::exception_ptr` without throwing or catching exceptions.

## Introduction

This is a follow up to two previous papers in this area:

Date | Link | Title
-----|------|------
Feb 7, 2018 | https://wg21.link/p0933 | Runtime introspection of exception_ptr
Oct 6, 2018 | https://wg21.link/p1066 | How to catch an exception_ptr without even try-ing

The papers above received positive feedback in earlier meetings. In 2018 Rapperswil meeting, EWG expressed strong desire in having such facility:

> Does EWG want a non throwing mechanism to get to the exception held by exception_ptr even if the performance was the same


SF| F| N| A | SA
--|--|--|---|---
16|8|1|0|0

LEWG looked at this at SanDiego 2018 and encouraged to come back after
addressing the following points:

* Remove the handle interface.
* Combine with P0933R0.
* Demonstrate field and implementation experience.
* Add wording
* Reach out to implementors and confirm this is implementable.

## Proposal at a glance

We introduce a single function `try_cast` that takes `std::exception_ptr` as an argument `e` and returns a pointer to an object referred to by `e`. 

```c++
template <typename T>
const std::remove_cvref_t<T>* 
try_cast(const std::exception_ptr& e) noexcept;
```

If `exception_ptr` is not empty and `std::remove_cvref_t<T>` is the type of the stored exception `E` or its unambiguous base, a const pointer to the stored exception is returned; otherwise returns `nullptr`.

**Example:**

Given the following error classes:
```c++
struct Foo {
    virtual ~Foo() {}
    int i = 1;
};
struct Bar : Foo, std::logic_error {
    Bar() : std::logic_error("This is Bar exception") {}
    int j = 2;
};
struct Baz : Bar {};
```
The execution of the following program
```c++
int main() {
    const auto eptr = std::make_exception_ptr(Baz());
    if (auto* x = try_cast<Baz>(eptr))
        printf("got '%s' what:'%s' i: %d j: %d\n", typeid(*x).name(), x->what(), x->i, x->j);
    if (auto* x = try_cast<Bar>(eptr))
        printf("got '%s' what:'%s' i: %d j: %d\n", typeid(*x).name(), x->what(), x->i, x->j);
    if (auto* x = try_cast<Foo>(eptr))
        printf("got '%s' i: %d\n", typeid(*x).name(), x->i);
}
```
results in the this output:

```
got '3Baz' what:'This is Bar exception' i: 1 j: 2
got '3Baz' what:'This is Bar exception' i: 1 j: 2
got '3Baz' i: 1```
```

## Rationale



## Proposed wording




## Motivation

In [std::execution](https://wg21.link/p2300) proposal, errors are propagated in a dedicated channel represented usually as `exception_ptr`.
Having a facility that 

Feb 7, 2018
https://wg21.link/p0933 Runtime introspection of exception_ptr
 
Oct 6, 2018
https://wg21.link/p1066 How to catch an exception_ptr without even try-ing

We had couple of proposals in this area in 2018:

Feb 7, 2018
https://wg21.link/p0933 Runtime introspection of exception_ptr

Oct 6, 2018
https://wg21.link/p1066 How to catch an exception_ptr without even try-ing

The sentiment for this proposal in EWG was overwhelming, yes, please. 
For example, in Rapperswil:

Does EWG want a non throwing mechanism to get to the exception held by exception_ptr even if the performance was the same


SF| F| N| A | SA
--|--|--|---|---
16|8|1|0|0

LEWG looked at this at SanDiego 2018 and said:

POLL: We should promise more committee time to P1066R1, knowing that our time is scarce and this will leave less time for other work.
SF| F| N| A | SA
--|--|--|---|---
4|8|1|1|0

Attendance: 14

POLL: We should promise more committee time to P1066R1 (w/o handle), knowing that our time is scarce and this will leave less time for other work.
SF| F| N| A | SA
--|--|--|---|---
3|8|1|2|0

Attendance: 14

Mark (N): I want to enable field experience.

CONSENSUS: 
- Bring a revision of P1066R1, with the guidance below, to LEWGI for refinement for C++23.

- Remove the handle interface.
- Combine with P0933R0.

- Demonstrate field and implementation experience.

- Add wording.

- Reach out to implementors and confirm this is implementable.

Snippets:
```c++
Runtime type introspection with std::exception ptr

exception_ptr.visit( [&](const std::runtime_error& exc) { cerr << exc.what() << endl; },
                     [&](const std::logic_error& exc) { cerr << exc.what() << endl; },
                     [&](const std::exception& exc) { cerr << exc.what() << endl; },
                     [&](const auto&) { std::terminate(); });

or possibly free functions, like we have with variant:


exception_ptr eptr;
std::visit(eptr, [&](const std::runtime_error& exc) { cerr << exc.what() << endl; },
                 [&](const std::logic_error& exc) { cerr << exc.what() << endl; },
                 [&](const std::exception& exc) { cerr << exc.what() << endl; },
                 [&](const auto&) { std::terminate(); });

if (auto* re = get_if<std::runtime_error>(eptr)) {
  ...
}

```
