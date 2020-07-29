
#include <gtest/gtest.h>
#include <ba/bytearray.hpp>

TEST(Serialization, FromString)
{
    std::string strict =
        "AABBCCDDEE1100";

    uint8_t strict_expected[] = {
        0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x11, 0x00
    };

    // Stricted
    {
        ba::bytearray v;

        ASSERT_NO_THROW(v.load_from_hex(strict));

        ASSERT_EQ(v.size(), 7);

        for (ba::bytearray<>::size_type i = 0; i < v.size(); ++i)
        {
            ASSERT_EQ(strict_expected[i], static_cast<uint8_t>(v[i]));
        }
    }
}