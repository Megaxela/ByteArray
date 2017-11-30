
#include <gtest/gtest.h>
#include <ByteArray.hpp>

TEST(Insert, ByteArray_ToEmpty)
{
    ByteArray b;

    uint8_t data[8] = {
        0, 1, 2, 3, 4, 5, 6, 7
    };

    ByteArray inserted(data, 8);

    ASSERT_NO_THROW(b.insert(0, inserted));

    ASSERT_EQ(b.size(), 8);

    for (ByteArray::size_type i = 0; i < b.size(); ++i)
    {
        ASSERT_EQ(b[i], inserted[i]);
    }
}

TEST(Insert, RawByteArray_ToEmpty)
{
    ByteArray b;

    uint8_t data[8] = {
        0, 1, 2, 3, 4, 5, 6, 7
    };

    ASSERT_NO_THROW(b.insert(0, data, 8));

    ASSERT_EQ(b.size(), 8);

    for (ByteArray::size_type i = 0; i < b.size(); ++i)
    {
        ASSERT_EQ(b[i], data[i]);
    }
}

TEST(Insert, ByteArray)
{
    uint8_t expected[16] = {
        1, 1, 2, 2, 0, 1, 2, 3, 4, 5, 6, 7, 3, 3, 4, 4
    };

    uint8_t initial[8] = {
        1, 1, 2, 2, 3, 3, 4, 4
    };

    ByteArray b(initial, 8);

    uint8_t data[8] = {
        0, 1, 2, 3, 4, 5, 6, 7
    };

    ByteArray inserted(data, 8);

    ASSERT_NO_THROW(b.insert(4, inserted));

    ASSERT_EQ(b.size(), 16);

    for (ByteArray::size_type i = 0; i < b.size(); ++i)
    {
        ASSERT_EQ(b[i], expected[i]);
    }
}

TEST(Insert, RawByteArray)
{
    uint8_t expected[16] = {
        1, 1, 2, 2, 0, 1, 2, 3, 4, 5, 6, 7, 3, 3, 4, 4
    };

    uint8_t initial[8] = {
        1, 1, 2, 2, 3, 3, 4, 4
    };

    ByteArray b(initial, 8);

    uint8_t data[8] = {
        0, 1, 2, 3, 4, 5, 6, 7
    };

    ByteArray inserted(data, 8);

    ASSERT_NO_THROW(b.insert(4, data, 8));

    ASSERT_EQ(b.size(), 16);

    for (ByteArray::size_type i = 0; i < b.size(); ++i)
    {
        ASSERT_EQ(b[i], expected[i]);
    }
}



TEST(Insert, Number8bit)
{
    uint8_t expected_be[9] = {
        1, 1, 2, 2, 0xDE, 3, 3, 4, 4
    };

    uint8_t expected_le[9] = {
        1, 1, 2, 2, 0xDE, 3, 3, 4, 4
    };

    uint8_t initial[8] = {
        1, 1, 2, 2, 3, 3, 4, 4
    };

    {
        ByteArray b(initial, 8);

        ASSERT_NO_THROW(b.insert<uint8_t>(
            4,
            0xDE,
            ByteArray::ByteOrder_LittleEndian
        ));

        ASSERT_EQ(b.size(), 9);

        for (ByteArray::size_type i = 0; i < b.size(); ++i)
        {
            ASSERT_EQ(b[i], expected_le[i]);
        }
    }

    {
        ByteArray b(initial, 8);

        ASSERT_NO_THROW(b.insert<uint8_t >(
            4,
            0xDE,
            ByteArray::ByteOrder_BigEndian
        ));

        ASSERT_EQ(b.size(), 9);

        std::cout << b.toHex() << std::endl;
        for (ByteArray::size_type i = 0; i < b.size(); ++i)
        {
            ASSERT_EQ(b[i], expected_be[i]);
        }
    }
}

TEST(Insert, Number16bit)
{
    uint8_t expected_be[10] = {
        1, 1, 2, 2, 0xDE, 0xAD, 3, 3, 4, 4
    };

    uint8_t expected_le[10] = {
        1, 1, 2, 2, 0xAD, 0xDE, 3, 3, 4, 4
    };

    uint8_t initial[8] = {
        1, 1, 2, 2, 3, 3, 4, 4
    };

    {
        ByteArray b(initial, 8);

        ASSERT_NO_THROW(b.insert<uint16_t>(
            4,
            0xDEAD,
            ByteArray::ByteOrder_LittleEndian
        ));

        ASSERT_EQ(b.size(), 10);

        for (ByteArray::size_type i = 0; i < b.size(); ++i)
        {
            ASSERT_EQ(b[i], expected_le[i]);
        }
    }

    {
        ByteArray b(initial, 8);

        ASSERT_NO_THROW(b.insert<uint16_t>(
            4,
            0xDEAD,
            ByteArray::ByteOrder_BigEndian
        ));

        ASSERT_EQ(b.size(), 10);

        for (ByteArray::size_type i = 0; i < b.size(); ++i)
        {
            ASSERT_EQ(b[i], expected_be[i]);
        }
    }
}

TEST(Insert, Number32bit)
{
    uint8_t expected_be[12] = {
        1, 1, 2, 2, 0xDE, 0xAD, 0xBE, 0xEF, 3, 3, 4, 4
    };

    uint8_t expected_le[12] = {
        1, 1, 2, 2, 0xEF, 0xBE, 0xAD, 0xDE, 3, 3, 4, 4
    };

    uint8_t initial[8] = {
        1, 1, 2, 2, 3, 3, 4, 4
    };

    {
        ByteArray b(initial, 8);

        ASSERT_NO_THROW(b.insert<uint32_t>(
            4,
            0xDEADBEEF,
            ByteArray::ByteOrder_LittleEndian
        ));

        ASSERT_EQ(b.size(), 12);

        for (ByteArray::size_type i = 0; i < b.size(); ++i)
        {
            ASSERT_EQ(b[i], expected_le[i]);
        }
    }

    {
        ByteArray b(initial, 8);

        ASSERT_NO_THROW(b.insert<uint32_t>(
            4,
            0xDEADBEEF,
            ByteArray::ByteOrder_BigEndian
        ));

        ASSERT_EQ(b.size(), 12);

        for (ByteArray::size_type i = 0; i < b.size(); ++i)
        {
            ASSERT_EQ(b[i], expected_be[i]);
        }
    }
}


TEST(Insert, Number64bit)
{
    uint8_t expected_be[16] = {
        1, 1, 2, 2, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 3, 3, 4, 4
    };

    uint8_t expected_le[16] = {
        1, 1, 2, 2, 0xEF, 0xBE, 0xAD, 0xDE, 0xEF, 0xBE, 0xAD, 0xDE, 3, 3, 4, 4
    };

    uint8_t initial[8] = {
        1, 1, 2, 2, 3, 3, 4, 4
    };

    {
        ByteArray b(initial, 8);

        ASSERT_NO_THROW(b.insert<uint64_t>(
            4,
            0xDEADBEEFDEADBEEF,
            ByteArray::ByteOrder_LittleEndian
        ));

        ASSERT_EQ(b.size(), 16);

        for (ByteArray::size_type i = 0; i < b.size(); ++i)
        {
            ASSERT_EQ(b[i], expected_le[i]);
        }
    }

    {
        ByteArray b(initial, 8);

        ASSERT_NO_THROW(b.insert<uint64_t>(
            4,
            0xDEADBEEFDEADBEEF,
            ByteArray::ByteOrder_BigEndian
        ));

        ASSERT_EQ(b.size(), 16);

        for (ByteArray::size_type i = 0; i < b.size(); ++i)
        {
            ASSERT_EQ(b[i], expected_be[i]);
        }
    }
}