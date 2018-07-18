cmake_minimum_required(VERSION 3.8)
project(ByteArrayTests)

add_subdirectory(googletest)
set(CMAKE_CXX_STANDARD 17)

include_directories(
        googletest/googletest/include
)

file(GLOB SOURCE_FILES *.cpp)

add_executable(ByteArrayTests ${SOURCE_FILES})

target_link_libraries(ByteArrayTests
        bytearray
        gtest
)