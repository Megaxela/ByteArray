cmake_minimum_required(VERSION 3.11)
project(bytearray)

option(BYTEARRAY_BUILD_TESTS "Build tests" Off)

set(CMAKE_CXX_STANDARD 17)

if (${BYTEARRAY_BUILD_TESTS})
    add_subdirectory(tests)
    add_subdirectory(benchmark)
endif()

add_library(bytearray STATIC
    include/bytearray_processor.hpp
    include/bytearray.hpp
    include/bytearray_view.hpp
)

target_include_directories(bytearray
    PUBLIC include
)

set_target_properties(bytearray PROPERTIES LINKER_LANGUAGE CXX)