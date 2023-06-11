add_executable(tests smoketest.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain conqueue)
catch_discover_tests(tests)
