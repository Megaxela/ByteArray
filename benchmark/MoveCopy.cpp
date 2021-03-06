#include <benchmark/benchmark.h>
#include <ba/bytearray.hpp>

static void copy(benchmark::State& state)
{
    ba::bytearray array;

    auto resultSize = state.range(0);

    array.push_back_multiple(
        0xFF, resultSize
    );

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(ba::bytearray<>(array));
    }

    state.SetComplexityN(resultSize);
}

static void move(benchmark::State& state)
{
    ba::bytearray array;

    auto resultSize = state.range(0);

    array.push_back_multiple(
        0xFF, resultSize
    );

    ba::bytearray copy;

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(copy = std::move(array));
    }

    state.SetComplexityN(resultSize);
}

BENCHMARK(copy)
    ->Range(1, 1 << 20)
    ->Complexity();

BENCHMARK(move)
    ->Range(1, 1 << 20)
    ->Complexity();