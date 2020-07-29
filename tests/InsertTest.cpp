#include <gtest/gtest.h>
#include <ba/bytearray.hpp>

TEST(Insert, Normal)
{
    uint8_t initial[] = {
        0xFF, 0xFF
    };

    uint8_t expect_le[] = {
        0xFF, 0xEF, 0xBE, 0xAD, 0xDE, 0xFF
    };

    uint8_t expect_be[] = {
        0xFF, 0xDE, 0xAD, 0xBE, 0xEF, 0xFF
    };

    {
        ba::bytearray array((std::byte*) initial, 2);

        array.insert<uint32_t>(1, 0xDEADBEEF, ba::endianness::big);

        ASSERT_EQ(array.size(), 6);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_be[i]);
        }
    }

    {
        ba::bytearray array((std::byte*) initial, 2);

        array.insert<uint32_t>(1, 0xDEADBEEF, ba::endianness::little);

        ASSERT_EQ(array.size(), 6);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_le[i]);
        }
    }
}

TEST(Insert, Part)
{
    uint8_t initial[] = {
        0xFF, 0xFF
    };

    uint8_t expect_le[] = {
        0xFF, 0xEF, 0xBE, 0xAD, 0xFF
    };

    uint8_t expect_be[] = {
        0xFF, 0xAD, 0xBE, 0xEF, 0xFF
    };

    {
        ba::bytearray array((std::byte*) initial, 2);

        array.insert_part<uint32_t>(1, 0xDEADBEEF, 3, ba::endianness::big);

        ASSERT_EQ(array.size(), 5);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_be[i]);
        }
    }

    {
        ba::bytearray array((std::byte*) initial, 2);

        array.insert_part<uint32_t>(1, 0xDEADBEEF, 3, ba::endianness::little);

        ASSERT_EQ(array.size(), 5);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_le[i]);
        }
    }
}

TEST(Insert, Multiple1)
{
    uint8_t initial[] = {
        0xFF, 0xFF
    };

    uint8_t expect_be[] = {
        0xFF, 0xDE, 0xAD, 0xDE, 0xAD, 0xFF
    };

    uint8_t expect_le[] = {
        0xFF, 0xAD, 0xDE, 0xAD, 0xDE, 0xFF
    };

    {
        ba::bytearray array(reinterpret_cast<const std::byte*>(initial), 2);

        array.insert_multiple<uint16_t>(
            1,
            0xDEAD,
            2,
            ba::endianness::big
        );

        ASSERT_EQ(array.size(), 6);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_be[i]);
        }
    }

    {
        ba::bytearray array(reinterpret_cast<const std::byte*>(initial), 2);

        array.insert_multiple<uint16_t>(
            1,
            0xDEAD,
            2,
            ba::endianness::little
        );

        ASSERT_EQ(array.size(), 6);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_le[i]);
        }
    }
}


TEST(Insert, Multiple2)
{
    uint8_t initial[] = {
        0xFF, 0xFF
    };

    uint8_t expect_be[] = {
        0xFF, 0xDE, 0xAD, 0xDE, 0xAD, 0xFF
    };

    uint8_t expect_le[] = {
        0xFF, 0xAD, 0xDE, 0xAD, 0xDE, 0xFF
    };

    std::vector<uint16_t> insertionData = {
        0xDEAD, 0xDEAD
    };

    {
        ba::bytearray array(reinterpret_cast<const std::byte*>(initial), 2);

        array.insert_multiple(
            1,
            insertionData.begin(),
            insertionData.end(),
            ba::endianness::big
        );

        ASSERT_EQ(array.size(), 6);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_be[i]);
        }
    }

    {
        ba::bytearray array(reinterpret_cast<const std::byte*>(initial), 2);

        array.insert_multiple(
            1,
            insertionData.begin(),
            insertionData.end(),
            ba::endianness::little
        );

        ASSERT_EQ(array.size(), 6);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_le[i]);
        }
    }
}