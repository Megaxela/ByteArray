project(ByteArrayBenchmark)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/benchmark/CMakeLists.txt")
    set(BENCHMARK_ENABLE_TESTING OFF)
    add_subdirectory(benchmark)
endif()

add_executable(ByteArrayBenchmark
        main.cpp
        AppendSpeed.cpp
        MoveCopy.cpp
        InsertSpeed.cpp
        )

target_link_libraries(ByteArrayBenchmark
    bytearray
    benchmark
    gtest
)