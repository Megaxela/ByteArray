
#include <gtest/gtest.h>
#include <ByteArray.hpp>

TEST(Serialization, FromString)
{
    std::string nonStrict =
        "AA BB CC DD EE 11 00";

    uint8_t nonStrictExpected[] = {
        0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x11, 0x00
    };

    std::string strict =
        "AABBCCDDEE1100";

    uint8_t strictExpected[] = {
        0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x11, 0x00
    };

    // Non stricted
    {
        ByteArray v;

        ASSERT_NO_THROW(v = ByteArray::fromHex(nonStrict, false));

        ASSERT_EQ(v.size(), 7);

        for (ByteArray::size_type i = 0; i < v.size(); ++i)
        {
            ASSERT_EQ(nonStrictExpected[i], v[i]);
        }
    }

    // Stricted
    {
        ByteArray v;

        ASSERT_NO_THROW(v = ByteArray::fromHex(strict, true));

        ASSERT_EQ(v.size(), 7);

        for (ByteArray::size_type i = 0; i < v.size(); ++i)
        {
            ASSERT_EQ(nonStrictExpected[i], v[i]);
        }

        ASSERT_THROW(
            v = ByteArray::fromHex(nonStrict, true),
            std::invalid_argument
        );
    }
}