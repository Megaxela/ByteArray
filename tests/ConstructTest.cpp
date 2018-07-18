
#include <gtest/gtest.h>
#include <bytearray.hpp>

TEST(Constructors, Default)
{
    bytearray b{};

    ASSERT_EQ(b.size(), 0);
    ASSERT_EQ(b.capacity(), 0);
}

TEST(Constructors, InitialFill)
{
    bytearray b(1024);

    ASSERT_EQ(b.size(), 1024);
    ASSERT_EQ(b.capacity(), 1024);
}

TEST(Constructors, InitialCapacity)
{
    bytearray b{};
    b.reserve(1024);

    ASSERT_EQ(b.size(), 0);
    ASSERT_EQ(b.capacity(), 1024);
}

TEST(Constructors, FromByteArray)
{
    uint8_t byteArray[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    };

    bytearray b(reinterpret_cast<std::byte*>(byteArray), 32);

    ASSERT_EQ(b.size(), 32);
    ASSERT_EQ(b.capacity(), 32);

    for (decltype(b)::size_type i = 0; i < b.size(); ++i)
    {
        ASSERT_EQ(byteArray[i], uint8_t(b[i]));
    }
}

TEST(Constructors, Copy)
{
    uint8_t byteArray[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    };

    bytearray b(reinterpret_cast<std::byte*>(byteArray), 32);

    bytearray copy = b;

    for (decltype(b)::size_type i = 0; i < b.size(); ++i)
    {
        ASSERT_EQ(b[i], copy[i]);
    }
}

TEST(Constructors, Move)
{
    uint8_t byteArray[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    };

    bytearray m(reinterpret_cast<std::byte*>(byteArray), 32);

    auto b = std::move(m);

    ASSERT_EQ(b.size(), 32);
    ASSERT_EQ(b.capacity(), 32);

    for (decltype(b)::size_type i = 0; i < b.size(); ++i)
    {
        ASSERT_EQ(byteArray[i], uint8_t(b[i]));
    }
}