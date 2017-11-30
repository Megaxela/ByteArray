cmake_minimum_required(VERSION 3.8)
project(ByteArrayTests)

add_subdirectory(googletest)

include_directories(
        googletest/googletest/include
        ../include
)

file(GLOB SOURCE_FILES *.cpp)

add_executable(ByteArrayTests ${SOURCE_FILES} ConstructTest.cpp AppendTest.cpp InsertTest.cpp SerializationTest.cpp UtilitiesTest.cpp ReadTest.cpp)

target_link_libraries(ByteArrayTests
        ByteArray
        gtest
)