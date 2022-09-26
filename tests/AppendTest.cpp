
#include <gtest/gtest.h>
#include <ba/bytearray.hpp>

TEST(Append, InitializerListUint8) {
    uint8_t data[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

    uint8_t expect[12] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xDE, 0xAD, 0xBE, 0xEF};

    {
        ba::bytearray b(reinterpret_cast<std::byte*>(data), 8);

        b.push_back_multiple<uint8_t>({0xDE, 0xAD, 0xBE, 0xEF}, ba::endianness::big);

        ASSERT_EQ(b.size(), 12);

        for (ba::bytearray<>::size_type i = 0; i < b.size(); ++i) {
            ASSERT_EQ(uint8_t(b[i]), expect[i]);
        }
    }

    {
        ba::bytearray b(reinterpret_cast<std::byte*>(data), 8);

        b.push_back_multiple<uint8_t>({0xDE, 0xAD, 0xBE, 0xEF}, ba::endianness::little);

        ASSERT_EQ(b.size(), 12);

        for (ba::bytearray<>::size_type i = 0; i < b.size(); ++i) {
            ASSERT_EQ(uint8_t(b[i]), expect[i]);
        }
    }
}

TEST(Append, InitializerListUint32) {
    uint8_t data[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

    uint8_t expect_be[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF};

    uint8_t expect_le[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xEF, 0xBE, 0xAD, 0xDE, 0xEF, 0xBE, 0xAD, 0xDE};

    {
        ba::bytearray b(reinterpret_cast<std::byte*>(data), 8);

        b.push_back_multiple<uint32_t>({0xDEADBEEF, 0xDEADBEEF}, ba::endianness::big);

        ASSERT_EQ(b.size(), 16);

        for (ba::bytearray<>::size_type i = 0; i < b.size(); ++i) {
            ASSERT_EQ(uint8_t(b[i]), expect_be[i]);
        }
    }

    {
        ba::bytearray b(reinterpret_cast<std::byte*>(data), 8);

        b.push_back_multiple<uint32_t>({0xDEADBEEF, 0xDEADBEEF}, ba::endianness::little);

        ASSERT_EQ(b.size(), 16);

        for (ba::bytearray<>::size_type i = 0; i < b.size(); ++i) {
            ASSERT_EQ(uint8_t(b[i]), expect_le[i]);
        }
    }
}

TEST(Append, Range) {
    uint8_t data[8] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    };

    uint8_t appendable[4] = {0xDE, 0xAD, 0xBE, 0xEF};

    ba::bytearray b(reinterpret_cast<std::byte*>(data), 8);

    b.push_back_multiple(appendable, appendable + 4);

    ASSERT_EQ(b.size(), 12);

    for (ba::bytearray<>::size_type i = 0; i < b.size(); ++i) {
        if (i >= 8) {
            ASSERT_EQ(uint8_t(b[i]), appendable[i - 8]);
        } else {
            ASSERT_EQ(uint8_t(b[i]), data[i]);
        }
    }
}

TEST(Append, Number8bit) {
    uint8_t data[8] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    };

    uint8_t expecting_be[9] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xDE};

    uint8_t expecting_le[9] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xDE};

    {
        ba::bytearray array(reinterpret_cast<std::byte*>(data), 8);

        array.push_back<uint8_t>(0xDE, ba::endianness::big);

        ASSERT_EQ(array.size(), 9);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i) {
            ASSERT_EQ(uint8_t(array[i]), expecting_be[i]);
        }
    }

    {
        ba::bytearray array(reinterpret_cast<std::byte*>(data), 8);

        array.push_back<uint8_t>(0xDE, ba::endianness::little);

        ASSERT_EQ(array.size(), 9);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i) {
            ASSERT_EQ(uint8_t(array[i]), expecting_le[i]);
        }
    }
}

TEST(Append, Number16bit) {
    uint8_t data[8] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    };

    uint8_t expecting_be[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xDE, 0xAD};

    uint8_t expecting_le[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xAD, 0xDE};

    {
        ba::bytearray array(reinterpret_cast<std::byte*>(data), 8);

        array.push_back<uint16_t>(0xDEAD, ba::endianness::big);

        ASSERT_EQ(array.size(), 10);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i) {
            ASSERT_EQ(uint8_t(array[i]), expecting_be[i]);
        }
    }

    {
        ba::bytearray array(reinterpret_cast<std::byte*>(data), 8);

        array.push_back<uint16_t>(0xDEAD, ba::endianness::little);

        ASSERT_EQ(array.size(), 10);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i) {
            ASSERT_EQ(uint8_t(array[i]), expecting_le[i]);
        }
    }
}

TEST(Append, Number32bit) {
    uint8_t data[8] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    };

    uint8_t expecting_be[12] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xDE, 0xAD, 0xBE, 0xEF};

    uint8_t expecting_le[12] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xEF, 0xBE, 0xAD, 0xDE};

    {
        ba::bytearray array(reinterpret_cast<std::byte*>(data), 8);

        array.push_back<uint32_t>(0xDEADBEEF, ba::endianness::big);

        ASSERT_EQ(array.size(), 12);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i) {
            ASSERT_EQ(uint8_t(array[i]), expecting_be[i]);
        }
    }

    {
        ba::bytearray array(reinterpret_cast<std::byte*>(data), 8);

        array.push_back<uint32_t>(0xDEADBEEF, ba::endianness::little);

        ASSERT_EQ(array.size(), 12);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i) {
            ASSERT_EQ(uint8_t(array[i]), expecting_le[i]);
        }
    }
}

TEST(Append, Number64bit) {
    uint8_t data[8] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    };

    uint8_t expecting_be[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF};

    uint8_t expecting_le[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xEF, 0xBE, 0xAD, 0xDE, 0xEF, 0xBE, 0xAD, 0xDE};

    {
        ba::bytearray array(reinterpret_cast<std::byte*>(data), 8);

        array.push_back<uint64_t>(0xDEADBEEFDEADBEEF, ba::endianness::big);

        ASSERT_EQ(array.size(), 16);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i) {
            ASSERT_EQ(uint8_t(array[i]), expecting_be[i]);
        }
    }

    {
        ba::bytearray array(reinterpret_cast<std::byte*>(data), 8);

        array.push_back<uint64_t>(0xDEADBEEFDEADBEEF, ba::endianness::little);

        ASSERT_EQ(array.size(), 16);

        for (ba::bytearray<>::size_type i = 0; i < array.size(); ++i) {
            ASSERT_EQ(uint8_t(array[i]), expecting_le[i]);
        }
    }
}
