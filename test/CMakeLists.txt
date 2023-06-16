add_executable(tests
    conqueue.test.cpp
    intrusive_list.test.cpp
    ring_buffer.test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain conqueue)
catch_discover_tests(tests)
