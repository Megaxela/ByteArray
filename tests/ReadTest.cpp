#include <gtest/gtest.h>
#include <ba/bytearray.hpp>

TEST(Read, Uint8)
{
    const uint8_t value = 0xDE;

    {
        ba::bytearray data{};

        data.push_back<uint8_t>(value, ba::endianness::big);

        ASSERT_EQ(
            data.read<uint8_t>(0, ba::endianness::big),
            value
        );
    }

    {
        ba::bytearray data{};

        data.push_back<uint8_t>(value, ba::endianness::little);

        ASSERT_EQ(
            data.read<uint8_t>(0, ba::endianness::little),
            value
        );
    }
}

TEST(Read, Uint32)
{
    const uint32_t value = 0xDEADBEEF;

    {
        ba::bytearray data{};

        data.push_back<uint32_t>(value, ba::endianness::big);

        ASSERT_EQ(
            data.read<uint32_t>(0, ba::endianness::big),
            value
        );
    }

    {
        ba::bytearray data{};

        data.push_back<uint32_t>(value, ba::endianness::little);

        ASSERT_EQ(
            data.read<uint32_t>(0, ba::endianness::little),
            value
        );
    }
}

TEST(Read, Uint64)
{
    const uint64_t value = 0xDEADBEEFFFEEFFEE;

    {
        ba::bytearray data{};

        data.push_back<uint64_t>(value, ba::endianness::big);

        ASSERT_EQ(
            data.read<uint64_t>(0, ba::endianness::big),
            value
        );
    }

    {
        ba::bytearray data{};

        data.push_back<uint64_t>(value, ba::endianness::little);

        ASSERT_EQ(
            data.read<uint64_t>(0, ba::endianness::little),
            value
        );
    }
}

TEST(Read, Int64)
{
    const int64_t value = 0xDEADBEEFFFEEFFEE;

    {
        ba::bytearray data{};

        data.push_back<int64_t>(value, ba::endianness::big);

        ASSERT_EQ(
            data.read<int64_t>(0, ba::endianness::big),
            value
        );
    }

    {
        ba::bytearray data{};

        data.push_back<int64_t>(value, ba::endianness::little);

        ASSERT_EQ(
            data.read<int64_t>(0, ba::endianness::little),
            value
        );
    }
}

struct CustomStructBase
{
    uint64_t a;
    uint32_t c;
    char z;

    char message[32];
};

struct CustomStructDerived : CustomStructBase
{
    char additional[64];

    bool operator==(const CustomStructDerived& rhs)
    {
        return
            rhs.a == a &&
            rhs.c == c &&
            rhs.z == z &&
            memcmp(rhs.message, message, 32) == 0 &&
            memcmp(rhs.additional, additional, 64) == 0;
    }
};

TEST(Read, Custom)
{
    CustomStructDerived value = {};

    value.a = 0xDEADDEADBEEFBEEF;
    value.c = 0xFEEDBEEF;
    value.z = '%';
    memcpy(value.message, "Sample message", 15);
    memcpy(value.additional, "Some actual long additional message", 35);

    {
        ba::bytearray data{};

        data.push_back(value, ba::endianness::big);

        ASSERT_TRUE(
            data.read<CustomStructDerived>(0, ba::endianness::big) == value
        );
    }

    {
        ba::bytearray data{};

        data.push_back(value, ba::endianness::little);

        ASSERT_TRUE(
            data.read<CustomStructDerived>(0, ba::endianness::little) == value
        );
    }
}

TEST(Read, Part)
{
    uint64_t value = 0x00ADBEEFFFEEAABB;

    {
        ba::bytearray data{};

        data.push_back_part(value, 7, ba::endianness::big);

        ASSERT_EQ(value, data.read_part<uint64_t>(0, 7, ba::endianness::big));
    }

    {
        ba::bytearray data{};

        data.push_back_part(value, 7, ba::endianness::little);

        ASSERT_EQ(value, data.read_part<uint64_t>(0, 7, ba::endianness::little));
    }
}
