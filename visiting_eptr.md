
# Visiting exception_ptr

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