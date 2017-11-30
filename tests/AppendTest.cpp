
#include <gtest/gtest.h>
#include <ByteArray.hpp>

TEST(Append, ByteArray)
{
    uint8_t data[8] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    };

    uint8_t appendable[4] = {
        0xDE, 0xAD, 0xBE, 0xEF
    };

    ByteArray b(data, 8);

    b.append(ByteArray(appendable, 4));

    ASSERT_EQ(b.size(), 12);

    for (ByteArray::size_type i = 0; i < b.size(); ++i)
    {
        if (i >= 8)
        {
            ASSERT_EQ(b[i], appendable[i - 8]);
        }
        else
        {
            ASSERT_EQ(b[i], data[i]);
        }
    }
}

TEST(Append, RawByteArray)
{
    uint8_t data[8] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    };

    uint8_t appendable[4] = {
        0xDE, 0xAD, 0xBE, 0xEF
    };

    ByteArray b(data, 8);

    b.append(appendable, 4);

    ASSERT_EQ(b.size(), 12);

    for (ByteArray::size_type i = 0; i < b.size(); ++i)
    {
        if (i >= 8)
        {
            ASSERT_EQ(b[i], appendable[i - 8]);
        }
        else
        {
            ASSERT_EQ(b[i], data[i]);
        }
    }
}

TEST(Append, Number8bit)
{
    uint8_t data[8] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    };

    uint8_t expecting_be[9] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0xDE
    };

    uint8_t expecting_le[9] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0xDE
    };

    {
        ByteArray array(data, 8);

        array.append<uint8_t>(0xDE, ByteArray::ByteOrder_BigEndian);

        ASSERT_EQ(array.size(), 9);
        ASSERT_EQ(array.length(), 9);

        for (ByteArray::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(array[i], expecting_be[i]);
        }
    }

    {
        ByteArray array(data, 8);

        array.append<uint8_t>(0xDE, ByteArray::ByteOrder_LittleEndian);

        ASSERT_EQ(array.size(), 9);
        ASSERT_EQ(array.length(), 9);

        for (ByteArray::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(array[i], expecting_le[i]);
        }
    }
}

TEST(Append, Number16bit)
{
    uint8_t data[8] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    };

    uint8_t expecting_be[10] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0xDE, 0xAD
    };

    uint8_t expecting_le[10] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0xAD, 0xDE
    };

    {
        ByteArray array(data, 8);

        array.append<uint16_t>(0xDEAD, ByteArray::ByteOrder_BigEndian);

        ASSERT_EQ(array.size(), 10);
        ASSERT_EQ(array.length(), 10);

        for (ByteArray::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(array[i], expecting_be[i]);
        }
    }

    {
        ByteArray array(data, 8);

        array.append<uint16_t>(0xDEAD, ByteArray::ByteOrder_LittleEndian);

        ASSERT_EQ(array.size(), 10);
        ASSERT_EQ(array.length(), 10);

        for (ByteArray::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(array[i], expecting_le[i]);
        }
    }
}

TEST(Append, Number32bit)
{
    uint8_t data[8] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    };

    uint8_t expecting_be[12] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0xDE, 0xAD, 0xBE, 0xEF
    };

    uint8_t expecting_le[12] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0xEF, 0xBE, 0xAD, 0xDE
    };

    {
        ByteArray array(data, 8);

        array.append<uint32_t>(0xDEADBEEF, ByteArray::ByteOrder_BigEndian);

        ASSERT_EQ(array.size(), 12);
        ASSERT_EQ(array.length(), 12);

        for (ByteArray::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(array[i], expecting_be[i]);
        }
    }

    {
        ByteArray array(data, 8);

        array.append<uint32_t>(0xDEADBEEF, ByteArray::ByteOrder_LittleEndian);

        ASSERT_EQ(array.size(), 12);
        ASSERT_EQ(array.length(), 12);

        for (ByteArray::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(array[i], expecting_le[i]);
        }
    }
}

TEST(Append, Number64bit)
{
    uint8_t data[8] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    };

    uint8_t expecting_be[16] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF
    };

    uint8_t expecting_le[16] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0xEF, 0xBE, 0xAD, 0xDE, 0xEF, 0xBE, 0xAD, 0xDE
    };

    {
        ByteArray array(data, 8);

        array.append<uint64_t>(0xDEADBEEFDEADBEEF, ByteArray::ByteOrder_BigEndian);

        ASSERT_EQ(array.size(), 16);
        ASSERT_EQ(array.length(), 16);

        for (ByteArray::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(array[i], expecting_be[i]);
        }
    }

    {
        ByteArray array(data, 8);

        array.append<uint64_t>(0xDEADBEEFDEADBEEF, ByteArray::ByteOrder_LittleEndian);

        ASSERT_EQ(array.size(), 16);
        ASSERT_EQ(array.length(), 16);

        for (ByteArray::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(array[i], expecting_le[i]);
        }
    }
}