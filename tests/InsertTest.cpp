#include <gtest/gtest.h>
#include <bytearray.hpp>

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
        bytearray array((std::byte*) initial, 2);

        array.insert<uint32_t>(1, 0xDEADBEEF, endianness::big);

        ASSERT_EQ(array.size(), 6);

        for (bytearray<>::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_be[i]);
        }
    }

    {
        bytearray array((std::byte*) initial, 2);

        array.insert<uint32_t>(1, 0xDEADBEEF, endianness::little);

        ASSERT_EQ(array.size(), 6);

        for (bytearray<>::size_type i = 0; i < array.size(); ++i)
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
        bytearray array((std::byte*) initial, 2);

        array.insert_part<uint32_t>(1, 0xDEADBEEF, 3, endianness::big);

        ASSERT_EQ(array.size(), 5);

        for (bytearray<>::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_be[i]);
        }
    }

    {
        bytearray array((std::byte*) initial, 2);

        array.insert_part<uint32_t>(1, 0xDEADBEEF, 3, endianness::little);

        ASSERT_EQ(array.size(), 5);

        for (bytearray<>::size_type i = 0; i < array.size(); ++i)
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
        bytearray array(reinterpret_cast<const std::byte*>(initial), 2);

        array.insert_multiple<uint16_t>(
            1,
            0xDEAD,
            2,
            endianness::big
        );

        ASSERT_EQ(array.size(), 6);

        for (bytearray<>::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_be[i]);
        }
    }

    {
        bytearray array(reinterpret_cast<const std::byte*>(initial), 2);

        array.insert_multiple<uint16_t>(
            1,
            0xDEAD,
            2,
            endianness::little
        );

        ASSERT_EQ(array.size(), 6);

        for (bytearray<>::size_type i = 0; i < array.size(); ++i)
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
        bytearray array(reinterpret_cast<const std::byte*>(initial), 2);

        array.insert_multiple(
            1,
            insertionData.begin(),
            insertionData.end(),
            endianness::big
        );

        ASSERT_EQ(array.size(), 6);

        for (bytearray<>::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_be[i]);
        }
    }

    {
        bytearray array(reinterpret_cast<const std::byte*>(initial), 2);

        array.insert_multiple(
            1,
            insertionData.begin(),
            insertionData.end(),
            endianness::little
        );

        ASSERT_EQ(array.size(), 6);

        for (bytearray<>::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_le[i]);
        }
    }
}