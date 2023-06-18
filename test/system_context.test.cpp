#include "std/experimental/system_context"
#include <catch2/catch_test_macros.hpp>

#include <stdio.h>

TEST_CASE("system_context: hello") {
  puts("Hello");
  puts("World");
}
