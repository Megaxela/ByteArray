#include <benchmark/benchmark.h>
#include <ByteArray.hpp>


template<typename T>
static void append(benchmark::State& state)
{
    auto resultSize = state.range(0);

    auto count = resultSize / sizeof(T);

    ByteArray byteArray(static_cast<ByteArray::size_type>(resultSize));

    for (auto _ : state)
    {
        byteArray.clear();

        for (int i = 0; i < count; ++i)
        {
            byteArray.append<T>(T(0xAA));
        }
    }

    state.SetComplexityN(resultSize);
}

template<typename T>
static void appendMultiple(benchmark::State& state)
{
    auto resultSize = state.range(0);

    auto count = resultSize / sizeof(T);

    ByteArray byteArray(static_cast<ByteArray::size_type>(resultSize));

    for (auto _ : state)
    {
        byteArray.clear();

        byteArray.appendMultiple<T>(T(0xAA), count);
    }

    state.SetComplexityN(resultSize);
}

static void appendByteArray(benchmark::State& state)
{
    auto size = state.range(0);

    ByteArray data(static_cast<ByteArray::size_type>(size));

    data.appendMultiple<uint8_t>(0x00, static_cast<ByteArray::size_type>(size));

    ByteArray byteArray(static_cast<ByteArray::size_type>(size));

    for (auto _ : state)
    {
        byteArray.clear();
        byteArray.append(data);
    }

    state.SetComplexityN(size);
}

static void appendPart(benchmark::State& state)
{
    uint64_t data = 0x1122334455667788;

    auto size = state.range(0);
    auto part = state.range(1);

    auto count = size / part;

    ByteArray byteArray(static_cast<ByteArray::size_type>(size));

    for (auto _ : state)
    {
        byteArray.clear();
        for (auto i = 0; i < count; ++i)
        {
            byteArray.appendPart(data, static_cast<ByteArray::size_type>(part));
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