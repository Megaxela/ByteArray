#include <benchmark/benchmark.h>
#include <bytearray.hpp>


template<typename T>
static void append(benchmark::State& state)
{
    auto resultSize = state.range(0);

    auto count = resultSize / sizeof(T);

    bytearray array;

    for (auto _ : state)
    {
        array.clear();

        for (int i = 0; i < count; ++i)
        {
            array.push_back<T>(T(0xAA));
        }
    }

    state.SetComplexityN(resultSize);
}

template<typename T>
static void appendMultiple(benchmark::State& state)
{
    auto resultSize = state.range(0);

    auto count = resultSize / sizeof(T);

    bytearray array;

    for (auto _ : state)
    {
        array.clear();

        array.push_back_multiple<T>(T(0xAA), count);
    }

    state.SetComplexityN(resultSize);
}

static void appendByteArray(benchmark::State& state)
{
    auto size = static_cast<size_t>(state.range(0));

    std::vector<std::byte> data;

    data.resize(size, std::byte(0x00));

    bytearray array;

    for (auto _ : state)
    {
        array.clear();
        array.push_back_multiple(data.begin(), data.end());
    }

    state.SetComplexityN(size);
}

static void appendPart(benchmark::State& state)
{
    uint64_t data = 0x1122334455667788;

    auto size = state.range(0);
    auto part = state.range(1);

    auto count = size / part;

    bytearray array;

    for (auto _ : state)
    {
        array.clear();
        for (auto i = 0; i < count; ++i)
        {
            array.push_back_part<uint64_t>(data, static_cast<std::size_t>(part));
        }
    }

    state.SetComplexityN(count);
}

BENCHMARK_TEMPLATE(append, uint8_t)
    ->Range(1, 1 << 20)
    ->Complexity();

BENCHMARK_TEMPLATE(append, uint16_t)
    ->Range(1, 1 << 20)
    ->Complexity();

BENCHMARK_TEMPLATE(append, uint32_t)
    ->Range(1, 1 << 20)
    ->Complexity();

BENCHMARK_TEMPLATE(append, uint64_t)
    ->Range(1, 1 << 20)
    ->Complexity();



BENCHMARK_TEMPLATE(appendMultiple, uint8_t)
    ->Range(1, 1 << 20)
    ->Complexity();

BENCHMARK_TEMPLATE(appendMultiple, uint16_t)
    ->Range(1, 1 << 20)
    ->Complexity();

BENCHMARK_TEMPLATE(appendMultiple, uint32_t)
    ->Range(1, 1 << 20)
    ->Complexity();

BENCHMARK_TEMPLATE(appendMultiple, uint64_t)
    ->Range(1, 1 << 20)
    ->Complexity();


BENCHMARK(appendByteArray)
    ->Range(1, 1 << 20)
    ->Complexity();

BENCHMARK(appendPart)
    ->RangeMultiplier(2)
    ->Ranges({{1, 1 << 20}, {1, 8}});