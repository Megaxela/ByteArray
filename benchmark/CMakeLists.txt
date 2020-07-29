project(bytearray_benchmark)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/benchmark/CMakeLists.txt")
    set(BENCHMARK_ENABLE_TESTING OFF)
    add_subdirectory(benchmark)
endif()

add_executable(bytearray_benchmark
    main.cpp
    AppendSpeed.cpp
    MoveCopy.cpp
    InsertSpeed.cpp
    CreationAndCopy.cpp
)

target_link_libraries(bytearray_benchmark
    bytearray
    benchmark
    gtest
)