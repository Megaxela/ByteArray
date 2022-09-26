cmake_minimum_required(VERSION 3.10)
project(bytearray)

option(BYTEARRAY_BUILD_TESTS "Build tests" Off)

set(CMAKE_CXX_STANDARD 17)

if (${BYTEARRAY_BUILD_TESTS})
    enable_testing()
    add_subdirectory(tests)
    add_subdirectory(benchmark)
endif()

add_library(bytearray STATIC
        include/ba/bytearray_reader.hpp
        include/ba/bytearray_processor.hpp
        include/ba/bytearray.hpp
        include/ba/bytearray_view.hpp
)

target_include_directories(bytearray PUBLIC include)
set_target_properties(bytearray PROPERTIES LINKER_LANGUAGE CXX)
