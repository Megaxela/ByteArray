cmake_minimum_required(VERSION 3.8)
project(ByteArray)

option(BYTEARRAY_BUILD_TESTS "Build tests" Off)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_CXX_FLAGS "-Werror \
#                     -ansi \
#                     -pedantic \
#                     -Wabi \
#                     -Wall \
#                     -Wextra \
#                     -Weffc++ \
#                     -Woverloaded-virtual \
#                     -Winit-self \
#                     -Wunreachable-code ")

if (${BYTEARRAY_BUILD_TESTS})
    add_subdirectory(tests)
endif()

include_directories(include)

set(INCLUDE_FILES
        include/ByteArray.hpp
)

set(SOURCE_FILES
        tests/main.cpp)

add_library(ByteArray
        ${SOURCE_FILES}
        ${INCLUDE_FILES}
)