#include <gtest/gtest.h>
#include <ByteArray.hpp>

TEST(Utilities, Reverse)
{
    ByteArray empty;

    ByteArray reversedEmpty = empty.reversed();

    ASSERT_EQ(reversedEmpty.length(), 0);
    ASSERT_EQ(reversedEmpty.capacity(), 0);

    uint8_t data[] = {
        0xDE, 0xAD, 0xBE, 0xEF,
        0xCA, 0xFE, 0xD0, 0x0D
    };

    ByteArray byteArray(data, 8);

    ByteArray reversed = byteArray.reversed();

    for (ByteArray::size_type i = 0; i < byteArray.size(); ++i)
    {
        ASSERT_EQ(reversed[i], data[8 - i - 1]);
    }
}

TEST(Utilities, Mid)
{
    uint8_t data[] = {
        0x11, 0x22, 0x33, 0x44,
        0x55, 0x66, 0x77, 0x88,
        0x99, 0xAA, 0xBB, 0xCC
    };

    ByteArray byteArray(data, 12);

    const ByteArray mid = byteArray.mid(4, 4);

    ASSERT_EQ(mid.length(), 4);

    for (ByteArray::size_type i = 0; i < mid.length(); ++i)
    {
        ASSERT_EQ(mid[i], data[4 + i]);
    }
}


TEST(Utilities, Cut)
{
    uint8_t data[] = {
        0x11, 0x22, 0x33, 0x44,
        0x55, 0x66, 0x77, 0x88,
        0x99, 0xAA, 0xBB, 0xCC
    };

    ByteArray byteArray(data, 12);

    const ByteArray cut = byteArray.cut(4, 8);

    ASSERT_EQ(cut.length(), 4);

    for (ByteArray::size_type i = 0; i < cut.length(); ++i)
    {
        ASSERT_EQ(cut[i], data[4 + i]);
    }
}

TEST(Utilities, Clear)
{
    uint8_t data[] = {
        0x11, 0x22, 0x33, 0x44,
        0x55, 0x66, 0x77, 0x88,
        0x99, 0xAA, 0xBB, 0xCC
    };

    ByteArray byteArray(data, 12);

    ASSERT_EQ(byteArray.length(), 12);

    byteArray.clear();

    ASSERT_EQ(byteArray.length(), 0);
    ASSERT_TRUE(byteArray.empty());
}