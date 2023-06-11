# fetch dependencies
include(FetchContent)

# do not build tests for subprojects
set(BUILD_TESTING OFF)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.3.2 # Feb 27, 2023
)

FetchContent_Declare(
  stdexec
  GIT_REPOSITORY https://github.com/NVIDIA/stdexec
  GIT_TAG        c4fac498b0c96dfc5cf4478e156f02fa4e13e49b # Jun 10, 2023
)

FetchContent_MakeAvailable(Catch2 stdexec)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
