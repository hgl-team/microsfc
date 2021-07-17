cmake_minimum_required(VERSION 3.14)

project(microsfc VERSION 1.1.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

include_directories(include ${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
file(GLOB SOURCES "src/*.cpp")
file(GLOB TEST_SOURCES "test/*.cpp" "test/*.h")


add_executable(microsfc-test ${SOURCES} ${TEST_SOURCES})
target_compile_options(microsfc-test PUBLIC -fprofile-arcs -ftest-coverage)
target_link_libraries(microsfc-test gtest gtest_main gmock pthread gcov)
add_test(microsfc-tests microsfc-test --gtest_output=xml:microsfc_test_report.xml)