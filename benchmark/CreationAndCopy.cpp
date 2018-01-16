#include <benchmark/benchmark.h>
#include <ByteArray.hpp>

static void creationEmpty(benchmark::State& state)
{
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(ByteArray());
    }
}

static void creationFromArrayOfBytes(benchmark::State& state)
{
    auto size = state.range(0);

    auto* data = new uint8_t[size];

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(ByteArray(data, static_cast<ByteArray::size_type>(size)));
    }

    delete[] data;

    state.SetComplexityN(size);
}

static void copyEmpty(benchmark::State& state)
{
    ByteArray byteArray;

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(ByteArray(byteArray));
    }
}

static void copyWithData(benchmark::State& state)
{
    auto size = static_cast<ByteArray::size_type>(state.range(0));

    ByteArray byteArray(size);

    byteArray.appendMultiple<uint8_t>(0x00, size);

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(ByteArray(byteArray));
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