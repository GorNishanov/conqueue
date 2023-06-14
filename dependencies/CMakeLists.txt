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

#if 0
FetchContent_Declare(
  stdexec
  GIT_REPOSITORY https://github.com/NVIDIA/stdexec
  GIT_TAG        c4fac498b0c96dfc5cf4478e156f02fa4e13e49b # Jun 10, 2023
)

FetchContent_MakeAvailable(Catch2 stdexec)
#else
FetchContent_Declare(
  unifex
  GIT_REPOSITORY https://github.com/facebookexperimental/libunifex
  GIT_TAG        5daf47f17825dd2c07465c0e606de81cd5ac788c # Jun 12, 2023
)

FetchContent_MakeAvailable(Catch2 unifex)
#endif

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
