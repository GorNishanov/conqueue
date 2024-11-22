# fetch dependencies
include(FetchContent)

# do not build tests or examples for subprojects
set(BUILD_TESTING OFF)
set(BUILD_EXAMPLES OFF)

set(STDEXEC_BUILD_TESTS OFF CACHE BOOL "Build stdexec tests" FORCE)
set(STDEXEC_BUILD_EXAMPLES OFF CACHE BOOL "Build stdexec examples" FORCE)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.3.2 # Feb 27, 2023
)

FetchContent_Declare(
  stdexec
  GIT_REPOSITORY https://github.com/NVIDIA/stdexec
  GIT_TAG        999a11e0a82f9e4d0cedb29ed0b0282543b30805 # Nov 18, 2024
)

FetchContent_MakeAvailable(Catch2 stdexec)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
