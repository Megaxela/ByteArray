cmake_minimum_required(VERSION 3.8)
project(bytearray_tests)

add_subdirectory(googletest)
set(CMAKE_CXX_STANDARD 17)

include_directories(
    googletest/googletest/include
)

file(GLOB SOURCE_FILES *Test.cpp)

add_executable(bytearray_tests ${SOURCE_FILES} main.cpp)

target_link_libraries(bytearray_tests
        bytearray
        gtest
)

add_test(
  NAME bytearray_tests
  COMMAND bytearray_tests
)
