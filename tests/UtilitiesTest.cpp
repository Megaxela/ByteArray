#include <gtest/gtest.h>
#include <bytearray.hpp>
#include <bytearray_view.hpp>

TEST(Utils, LoadFromHexString)
{
    bytearray array{};

    uint8_t expect[] = {
        0xAA, 0xFF, 0xEE, 0xDD
    };

    ASSERT_TRUE(array.load_from_hex(std::string_view("AAFFEEDD")));

    ASSERT_EQ(array.size(), 4);

    for (decltype(array)::size_type i = 0; i < array.size(); ++i)
    {
        ASSERT_EQ(uint8_t(array[i]), expect[i]);
    }

    ASSERT_TRUE(array.load_from_hex(std::string_view("AA FFEEDD")));

    ASSERT_EQ(array.size(), 4);

    for (decltype(array)::size_type i = 0; i < array.size(); ++i)
    {
        ASSERT_EQ(uint8_t(array[i]), expect[i]);
    }

    ASSERT_TRUE(array.load_from_hex(std::string_view("AA FF EE DD")));

    ASSERT_EQ(array.size(), 4);

    for (decltype(array)::size_type i = 0; i < array.size(); ++i)
    {
        ASSERT_EQ(uint8_t(array[i]), expect[i]);
    }

    ASSERT_FALSE(array.load_from_hex(std::string_view("AAFFEE WW")));

    ASSERT_EQ(array.size(), 4);

    for (decltype(array)::size_type i = 0; i < array.size(); ++i)
    {
        ASSERT_EQ(uint8_t(array[i]), expect[i]);
    }
}

TEST(Utils, Literal)
{
    auto ba = "DEADBEEF"_ba;

    uint8_t expect[] = {
        0xDE, 0xAD, 0xBE, 0xEF
    };

    ASSERT_EQ(ba.size(), 4);

    for (decltype(ba)::size_type i = 0; i < ba.size(); ++i)
    {
        ASSERT_EQ(uint8_t(ba[i]), expect[i]);
    }
}

TEST(Utils, ToString)
{
    auto ba = "DEADBEEF"_ba;

    auto string = std::to_string(ba);

    ASSERT_EQ(string, "DEADBEEF");
}

TEST(Utils, ViewToString)
{
    auto ba = "DEADBEEF"_ba;

    bytearray_view view(ba, 1, 2);

    auto string = std::to_string(view);

    ASSERT_EQ(string, "ADBE");
}