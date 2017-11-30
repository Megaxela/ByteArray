
#include <gtest/gtest.h>
#include <ByteArray.hpp>

TEST(Read, Number8bit)
{
    uint8_t data_be[] = {
        0xDE
    };

    uint8_t data_le[] = {
        0xDE
    };


    uint8_t number = 0xDE;

    {
        ByteArray byteArray(data_be, 1);

        ASSERT_EQ(
            byteArray.read<uint8_t>(0,ByteArray::ByteOrder_BigEndian),
            number
        );
    }

    {
        ByteArray byteArray(data_le, 1);

        ASSERT_EQ(
            byteArray.read<uint8_t>(0,ByteArray::ByteOrder_LittleEndian),
            number
        );
    }
}

TEST(Read, Number16bit)
{
    uint8_t data_be[] = {
        0xDE, 0xAD
    };

    uint8_t data_le[] = {
        0xAD, 0xDE
    };


    uint16_t number = 0xDEAD;

    {
        ByteArray byteArray(data_be, 2);

        ASSERT_EQ(
            byteArray.read<uint16_t>(0,ByteArray::ByteOrder_BigEndian),
            number
        );
    }

    {
        ByteArray byteArray(data_le, 2);

        ASSERT_EQ(
            byteArray.read<uint16_t>(0,ByteArray::ByteOrder_LittleEndian),
            number
        );
    }
}


TEST(Read, Number32bit)
{
    uint8_t data_be[] = {
        0xDE, 0xAD, 0xBE, 0xEF
    };

    uint8_t data_le[] = {
        0xEF, 0xBE, 0xAD, 0xDE
    };


    uint32_t number = 0xDEADBEEF;

    {
        ByteArray byteArray(data_be, 4);

        ASSERT_EQ(
            byteArray.read<uint32_t>(0,ByteArray::ByteOrder_BigEndian),
            number
        );
    }

    {
        ByteArray byteArray(data_le, 4);

        ASSERT_EQ(
            byteArray.read<uint32_t>(0,ByteArray::ByteOrder_LittleEndian),
            number
        );
    }
}


TEST(Read, Number64bit)
{
    uint8_t data_be[] = {
        0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF
    };

    uint8_t data_le[] = {
        0xEF, 0xBE, 0xAD, 0xDE, 0xEF, 0xBE, 0xAD, 0xDE
    };


    uint64_t number = 0xDEADBEEFDEADBEEF;

    {
        ByteArray byteArray(data_be, 8);

        ASSERT_EQ(
            byteArray.read<uint64_t>(0,ByteArray::ByteOrder_BigEndian),
            number
        );
    }

    {
        ByteArray byteArray(data_le, 8);

        ASSERT_EQ(
            byteArray.read<uint64_t>(0,ByteArray::ByteOrder_LittleEndian),
            number
        );
    }
}

