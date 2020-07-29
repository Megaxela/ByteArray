#include <gtest/gtest.h>
#include <ba/bytearray.hpp>

uint8_t initialData[8] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x01, 0x02
};

TEST(Set, UInt8)
{
    uint8_t expect[8] = {
        0x01, 0x02, 0xDE, 0x04, 0x05, 0x06, 0x01, 0x02
    };

    {
        ba::bytearray data(reinterpret_cast<std::byte*>(initialData), 8);

        data.set<uint8_t>(2, 0xDE, ba::endianness::big);

        ASSERT_EQ(data.size(), 8);

        for (ba::bytearray<>::size_type i = 0; i < data.size(); ++i)
        {
            ASSERT_EQ(uint8_t(data[i]), expect[i]);
        }
    }

    {
        ba::bytearray data(reinterpret_cast<std::byte*>(initialData), 8);

        data.set<uint8_t>(2, 0xDE, ba::endianness::little);

        ASSERT_EQ(data.size(), 8);

        for (ba::bytearray<>::size_type i = 0; i < data.size(); ++i)
        {
            ASSERT_EQ(uint8_t(data[i]), expect[i]);
        }
    }
}

TEST(Set, UInt16)
{
    uint8_t expect_be[8] = {
        0x01, 0x02, 0xDE, 0xAD, 0x05, 0x06, 0x01, 0x02
    };
    uint8_t expect_le[8] = {
        0x01, 0x02, 0xAD, 0xDE, 0x05, 0x06, 0x01, 0x02
    };

    {
        ba::bytearray data(reinterpret_cast<std::byte*>(initialData), 8);

        data.set<uint16_t>(2, 0xDEAD, ba::endianness::big);

        ASSERT_EQ(data.size(), 8);

        for (ba::bytearray<>::size_type i = 0; i < data.size(); ++i)
        {
            ASSERT_EQ(uint8_t(data[i]), expect_be[i]);
        }
    }

    {
        ba::bytearray data(reinterpret_cast<std::byte*>(initialData), 8);

        data.set<uint16_t>(2, 0xDEAD, ba::endianness::little);

        ASSERT_EQ(data.size(), 8);

        for (ba::bytearray<>::size_type i = 0; i < data.size(); ++i)
        {
            ASSERT_EQ(uint8_t(data[i]), expect_le[i]);
        }
    }
}

TEST(Set, UInt32)
{
    uint8_t expect_be[8] = {
        0x01, 0x02, 0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x02
    };
    uint8_t expect_le[8] = {
        0x01, 0x02, 0xEF, 0xBE, 0xAD, 0xDE, 0x01, 0x02
    };

    {
        ba::bytearray data(reinterpret_cast<std::byte*>(initialData), 8);

        data.set<uint32_t>(2, 0xDEADBEEF, ba::endianness::big);

        ASSERT_EQ(data.size(), 8);

        for (ba::bytearray<>::size_type i = 0; i < data.size(); ++i)
        {
            ASSERT_EQ(uint8_t(data[i]), expect_be[i]);
        }
    }

    {
        ba::bytearray data(reinterpret_cast<std::byte*>(initialData), 8);

        data.set<uint32_t>(2, 0xDEADBEEF, ba::endianness::little);

        ASSERT_EQ(data.size(), 8);

        for (ba::bytearray<>::size_type i = 0; i < data.size(); ++i)
        {
            ASSERT_EQ(uint8_t(data[i]), expect_le[i]);
        }
    }
}

TEST(Set, UInt64)
{
    uint8_t expect_be[8] = {
        0xDE, 0xAD, 0xBE, 0xEF, 0xEE, 0xAA, 0xBB, 0xCC
    };
    uint8_t expect_le[8] = {
        0xCC, 0xBB, 0xAA, 0xEE, 0xEF, 0xBE, 0xAD, 0xDE
    };

    {
        ba::bytearray data(reinterpret_cast<std::byte*>(initialData), 8);

        data.set<uint64_t>(0, 0xDEADBEEFEEAABBCC, ba::endianness::big);

        ASSERT_EQ(data.size(), 8);

        for (ba::bytearray<>::size_type i = 0; i < data.size(); ++i)
        {
            ASSERT_EQ(uint8_t(data[i]), expect_be[i]);
        }
    }

    {
        ba::bytearray data(reinterpret_cast<std::byte*>(initialData), 8);

        data.set<uint64_t>(0, 0xDEADBEEFEEAABBCC, ba::endianness::little);

        ASSERT_EQ(data.size(), 8);

        for (ba::bytearray<>::size_type i = 0; i < data.size(); ++i)
        {
            ASSERT_EQ(uint8_t(data[i]), expect_le[i]);
        }
    }
}

