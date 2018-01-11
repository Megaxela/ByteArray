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

set(INCLUDE_FILES
        include/ByteArray.hpp
)

set(SOURCE_FILES)

add_library(ByteArray
        ${SOURCE_FILES}
        ${INCLUDE_FILES}
)

target_include_directories(ByteArray PUBLIC
        include
)

set_target_properties(ByteArray PROPERTIES LINKER_LANGUAGE CXX)