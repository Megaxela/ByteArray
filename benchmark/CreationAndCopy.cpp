#include <benchmark/benchmark.h>
#include <ba/bytearray.hpp>

static void creationEmpty(benchmark::State& state)
{
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(ba::bytearray());
    }
}

static void creationFromArrayOfBytes(benchmark::State& state)
{
    auto size = state.range(0);

    auto* data = new std::byte[size];

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(ba::bytearray(data, size));
    }

    delete[] data;

    state.SetComplexityN(size);
}

static void copyEmpty(benchmark::State& state)
{
    ba::bytearray byteArray;

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(ba::bytearray(byteArray));
    }
}

static void copyWithData(benchmark::State& state)
{
    auto size = static_cast<ba::bytearray<>::size_type>(state.range(0));

    ba::bytearray byteArray(size);

    byteArray.push_back_multiple<uint8_t>(0x00, size);

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(ba::bytearray(byteArray));
    }

    state.SetComplexityN(size);
}

BENCHMARK(creationEmpty);

BENCHMARK(creationFromArrayOfBytes)
    ->Range(1, 1 << 20)
    ->Complexity();

BENCHMARK(copyEmpty);

BENCHMARK(copyWithData)
    ->Range(1, 1 << 20)
    ->Complexity();