cmake_minimum_required(VERSION 3.10)

project(microsfc VERSION 1.1.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)


include_directories(include ${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
file(GLOB SOURCES "src/*.cpp")
file(GLOB TEST_SOURCES "test/*.cpp" "test/*.h")


add_executable(microsfc-test ${SOURCES} ${TEST_SOURCES})
target_link_libraries(microsfc-test gtest gtest_main gmock pthread)
add_test(microsfc-tests microsfc-test)