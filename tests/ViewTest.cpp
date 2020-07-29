#include <gtest/gtest.h>
#include <ba/bytearray.hpp>
#include <ba/bytearray_view.hpp>

TEST(View, ConstructorByteArray)
{
    ba::bytearray array{};

    ba::bytearray_view view(array);
}

TEST(View, ConstructorByteArrayProcessor)
{
    std::vector<std::byte> container;

    ba::bytearray_processor processor(container);

    ba::bytearray_view view(processor);
}

TEST(View, IndexOperator)
{
    ba::bytearray array{};

    array.push_back<uint64_t>(0xDEADBEEFAABBCCDD);

    ba::bytearray_view view(array, 1, 6); // 0xADBEEFAABBCC

    uint8_t expect[] = {
        0xAD, 0xBE, 0xEF, 0xAA, 0xBB, 0xCC
    };

    ASSERT_EQ(view.size(), 6);

    for (decltype(view)::size_type i = 0; i < 6; ++i)
    {
        ASSERT_EQ(uint8_t(view[i]), expect[i]);
    }
}

TEST(View, Iterators)
{
    ba::bytearray array{};

    array.push_back<uint64_t>(0xDEADBEEFAABBCCDD);

    ba::bytearray_view view(array, 1, 6); // 0xADBEEFAABBCC

    uint8_t expect[] = {
        0xAD, 0xBE, 0xEF, 0xAA, 0xBB, 0xCC
    };

    ASSERT_EQ(view.size(), 6);

    for (auto iter = view.begin(),
              end = view.end();
         iter != end;
         ++iter)
    {
        ASSERT_EQ(uint8_t(*iter), expect[std::distance(view.begin(), iter)]);
    }
}

TEST(View, ConstantIterators)
{
    ba::bytearray array{};

    array.push_back<uint64_t>(0xDEADBEEFAABBCCDD);

    const ba::bytearray_view view(array, 1, 6); // 0xADBEEFAABBCC

    uint8_t expect[] = {
        0xAD, 0xBE, 0xEF, 0xAA, 0xBB, 0xCC
    };

    ASSERT_EQ(view.size(), 6);

    for (auto iter = view.cbegin(),
              end = view.cend();
         iter != end;
         ++iter)
    {
        ASSERT_EQ(uint8_t(*iter), expect[std::distance(view.cbegin(), iter)]);
    }
}

TEST(View, ReverseIterators)
{
    ba::bytearray array{};

    array.push_back<uint64_t>(0xDEADBEEFAABBCCDD);

    ba::bytearray_view view(array, 1, 6); // 0xADBEEFAABBCC

    uint8_t expect[] = {
        0xCC, 0xBB, 0xAA, 0xEF, 0xBE, 0xAD
    };

    ASSERT_EQ(view.size(), 6);

    for (auto iter = view.rbegin(),
              end = view.rend();
         iter != end;
         ++iter)
    {
        ASSERT_EQ(uint8_t(*iter), expect[std::distance(view.rbegin(), iter)]);
    }
}

TEST(View, ConstantReverseIterators)
{
    ba::bytearray array{};

    array.push_back<uint64_t>(0xDEADBEEFAABBCCDD);

    const ba::bytearray_view view(array, 1, 6); // 0xADBEEFAABBCC

    uint8_t expect[] = {
        0xCC, 0xBB, 0xAA, 0xEF, 0xBE, 0xAD
    };

    ASSERT_EQ(view.size(), 6);

    for (auto iter = view.crbegin(),
               end = view.crend();
         iter != end;
         ++iter)
    {
        ASSERT_EQ(uint8_t(*iter), expect[std::distance(view.crbegin(), iter)]);
    }
}

TEST(View, PushBackUint8)
{
    ba::bytearray array{};

    array.push_back<uint64_t>(0xDEADBEEFAABBCCDD);

    ba::bytearray_view view(array, 1, 6);

    uint8_t expect_view[] = {
        0xAD, 0xBE, 0xEF, 0xAA, 0xBB, 0xCC, 0xFF
    };

    uint8_t expect_array[] = {
        0xDE, 0xAD, 0xBE, 0xEF, 0xAA, 0xBB, 0xCC, 0xFF, 0xDD
    };

    view.push_back<uint8_t>(0xFF);

    ASSERT_EQ(view.size(), 7);
    ASSERT_EQ(array.size(), 9);

    for (decltype(view)::size_type i = 0; i < view.size(); ++i)
    {
        ASSERT_EQ(uint8_t(view[i]), expect_view[i]);
    }

    for (decltype(array)::size_type i = 0; i < array.size(); ++i)
    {
        ASSERT_EQ(uint8_t(array[i]), expect_array[i]);
    }
}

TEST(View, PushBackPart)
{
    uint8_t init[] {
        0xFF, 0xFF, 0xFF, 0xFF
    };

    uint8_t expect_le[] = {
        0xFF, 0xFF, 0xFF, 0xEF, 0xBE, 0xAD, 0xFF
    };

    uint8_t expect_le_view[] = {
        0xFF, 0xFF, 0xEF, 0xBE, 0xAD
    };

    uint8_t expect_be[] = {
        0xFF, 0xFF, 0xFF, 0xAD, 0xBE, 0xEF, 0xFF
    };

    uint8_t expect_be_view[] = {
        0xFF, 0xFF, 0xAD, 0xBE, 0xEF
    };

    {
        ba::bytearray array(reinterpret_cast<const std::byte*>(init), 4);

        ba::bytearray_view view(array, 1, 2);

        view.push_back_part<uint32_t>(0xDEADBEEF, 3, ba::endianness::big);

        ASSERT_EQ(array.size(), 7);
        ASSERT_EQ(view.size(), 5);

        for (decltype(view)::size_type i = 0; i < view.size(); ++i)
        {
            ASSERT_EQ(uint8_t(view[i]), expect_be_view[i]);
        }

        for (decltype(array)::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_be[i]);
        }
    }

    {
        ba::bytearray array(reinterpret_cast<const std::byte*>(init), 4);

        ba::bytearray_view view(array, 1, 2);

        view.push_back_part<uint32_t>(0xDEADBEEF, 3, ba::endianness::little);

        ASSERT_EQ(array.size(), 7);
        ASSERT_EQ(view.size(), 5);

        for (decltype(view)::size_type i = 0; i < view.size(); ++i)
        {
            ASSERT_EQ(uint8_t(view[i]), expect_le_view[i]);
        }

        for (decltype(array)::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_le[i]);
        }
    }
}

TEST(View, PushBackMultiple1)
{
    uint8_t init[] {
        0xFF, 0xFF, 0xFF, 0xFF
    };

    uint8_t expect_be[] = {
        0xFF, 0xFF, 0xFF, 0xDE, 0xAD, 0xDE, 0xAD, 0xFF
    };

    uint8_t expect_be_view[] = {
        0xFF, 0xFF, 0xDE, 0xAD, 0xDE, 0xAD
    };

    uint8_t expect_le[] = {
        0xFF, 0xFF, 0xFF, 0xAD, 0xDE, 0xAD, 0xDE, 0xFF
    };

    uint8_t expect_le_view[] = {
        0xFF, 0xFF, 0xAD, 0xDE, 0xAD, 0xDE
    };

    {
        ba::bytearray array(reinterpret_cast<const std::byte*>(init), 4);

        ba::bytearray_view view(array, 1, 2);

        view.push_back_multiple<uint16_t>(
            0xDEAD, 2, ba::endianness::big
        );

        ASSERT_EQ(array.size(), 8);
        ASSERT_EQ(view.size(), 6);

        for (decltype(array)::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_be[i]);
        }

        for (decltype(view)::size_type i = 0; i < view.size(); ++i)
        {
            ASSERT_EQ(uint8_t(view[i]), expect_be_view[i]);
        }
    }

    {
        ba::bytearray array(reinterpret_cast<const std::byte*>(init), 4);

        ba::bytearray_view view(array, 1, 2);

        view.push_back_multiple<uint16_t>(
            0xDEAD, 2, ba::endianness::little
        );

        ASSERT_EQ(array.size(), 8);
        ASSERT_EQ(view.size(), 6);

        for (decltype(array)::size_type i = 0; i < array.size(); ++i)
        {
            ASSERT_EQ(uint8_t(array[i]), expect_le[i]);
        }

        for (decltype(view)::size_type i = 0; i < view.size(); ++i)
        {
            ASSERT_EQ(uint8_t(view[i]), expect_le_view[i]);
        }
    }
}

TEST(View, Empty)
{
    ba::bytearray array{};

    ba::bytearray_view view(array);

    ASSERT_TRUE(view.empty());

    view.push_back<uint8_t>(0xFF);

    ASSERT_FALSE(view.empty());
}

TEST(View, Comparison)
{
    ba::bytearray array1{};
    ba::bytearray array2{};

    ba::bytearray_view view1(array1);
    ba::bytearray_view view2(array2);

    ASSERT_TRUE(view1 == view2);

    view1.push_back<uint64_t>(0xDDEEAADDBBEEEEFF);
    view2.push_back<uint64_t>(0xDDEEAADDBBEEEEFF);

    ASSERT_TRUE(view1 == view2);

    auto tmp = view1[0];

    view1[0] = std::byte(0xAA);

    ASSERT_FALSE(view1 == view2);

    view1[0] = tmp;

    ASSERT_TRUE(view1 == view2);

    view1.push_back<uint8_t>(0xAA);

    ASSERT_FALSE(view1 == view2);
}

TEST(View, InsertNormal)
{
    uint8_t initial[] = {
        0xFF, 0xFF, 0xFF, 0xFF
    };

    uint8_t expect_le[] = {
        0xFF, 0xEF, 0xBE, 0xAD, 0xDE, 0xFF
    };

    uint8_t expect_be[] = {
        0xFF, 0xDE, 0xAD, 0xBE, 0xEF, 0xFF
    };

    {
        ba::bytearray array((std::byte*) initial, 4);

        ba::bytearray_view view(array, 1, 2);

        view.insert<uint32_t>(1, 0xDEADBEEF, ba::endianness::big);

        ASSERT_EQ(view.size(), 6);

        for (ba::bytearray<>::size_type i = 0; i < view.size(); ++i)
        {
            ASSERT_EQ(uint8_t(view[i]), expect_be[i]);
        }
    }

    {
        ba::bytearray array((std::byte*) initial, 4);

        ba::bytearray_view view(array, 1, 2);

        view.insert<uint32_t>(1, 0xDEADBEEF, ba::endianness::little);

        ASSERT_EQ(view.size(), 6);

        for (ba::bytearray<>::size_type i = 0; i < view.size(); ++i)
        {
            ASSERT_EQ(uint8_t(view[i]), expect_le[i]);
        }
    }
}

TEST(View, InsertPart)
{
    uint8_t initial[] = {
        0xFF, 0xFF, 0xFF, 0xFF
    };

    uint8_t expect_le[] = {
        0xFF, 0xEF, 0xBE, 0xAD, 0xFF
    };

    uint8_t expect_be[] = {
        0xFF, 0xAD, 0xBE, 0xEF, 0xFF
    };

    {
        ba::bytearray array((std::byte*) initial, 4);

        ba::bytearray_view view(array, 1, 2);

        view.insert_part<uint32_t>(1, 0xDEADBEEF, 3, ba::endianness::big);

        ASSERT_EQ(view.size(), 5);

        for (ba::bytearray<>::size_type i = 0; i < view.size(); ++i)
        {
            ASSERT_EQ(uint8_t(view[i]), expect_be[i]);
        }
    }

    {
        ba::bytearray array((std::byte*) initial, 4);

        ba::bytearray_view view(array, 1, 2);

        view.insert_part<uint32_t>(1, 0xDEADBEEF, 3, ba::endianness::little);

        ASSERT_EQ(view.size(), 5);

        for (ba::bytearray<>::size_type i = 0; i < view.size(); ++i)
        {
            ASSERT_EQ(uint8_t(view[i]), expect_le[i]);
        }
    }
}

TEST(View, InsertMultiple1)
{
    uint8_t initial[] = {
        0xFF, 0xFF, 0xFF, 0xFF
    };

    uint8_t expect_be[] = {
        0xFF, 0xDE, 0xAD, 0xDE, 0xAD, 0xFF
    };

    uint8_t expect_le[] = {
        0xFF, 0xAD, 0xDE, 0xAD, 0xDE, 0xFF
    };

    {
        ba::bytearray array(reinterpret_cast<const std::byte*>(initial), 4);

        ba::bytearray_view view(array, 1, 2);

        view.insert_multiple<uint16_t>(
            1,
            0xDEAD,
            2,
            ba::endianness::big
        );

        ASSERT_EQ(view.size(), 6);

        for (ba::bytearray<>::size_type i = 0; i < view.size(); ++i)
        {
            ASSERT_EQ(uint8_t(view[i]), expect_be[i]);
        }
    }

    {
        ba::bytearray array(reinterpret_cast<const std::byte*>(initial), 4);

        ba::bytearray_view view(array, 1, 2);

        view.insert_multiple<uint16_t>(
            1,
            0xDEAD,
            2,
            ba::endianness::little
        );

        ASSERT_EQ(view.size(), 6);

        for (ba::bytearray<>::size_type i = 0; i < view.size(); ++i)
        {
            ASSERT_EQ(uint8_t(view[i]), expect_le[i]);
        }
    }
}


TEST(View, InsertMultiple2)
{
    uint8_t initial[] = {
        0xFF, 0xFF, 0xFF, 0xFF
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
        ba::bytearray array(reinterpret_cast<const std::byte*>(initial), 4);

        ba::bytearray_view view(array, 1, 2);

        view.insert_multiple(
            1,
            insertionData.begin(),
            insertionData.end(),
            ba::endianness::big
        );

        ASSERT_EQ(view.size(), 6);

        for (ba::bytearray<>::size_type i = 0; i < view.size(); ++i)
        {
            ASSERT_EQ(uint8_t(view[i]), expect_be[i]);
        }
    }

    {
        ba::bytearray array(reinterpret_cast<const std::byte*>(initial), 4);

        ba::bytearray_view view(array, 1, 2);

        view.insert_multiple(
            1,
            insertionData.begin(),
            insertionData.end(),
            ba::endianness::little
        );

        ASSERT_EQ(view.size(), 6);

        for (ba::bytearray<>::size_type i = 0; i < view.size(); ++i)
        {
            ASSERT_EQ(uint8_t(view[i]), expect_le[i]);
        }
    }
}


TEST(View, ReadUint8)
{
    const uint8_t value = 0xDE;

    {
        ba::bytearray data{};

        ba::bytearray_view view(data, 0, 0);

        view.push_back<uint8_t>(value, ba::endianness::big);

        ASSERT_EQ(
            view.read<uint8_t>(0, ba::endianness::big),
            value
        );
    }

    {
        ba::bytearray data{};

        ba::bytearray_view view(data, 0, 0);

        view.push_back<uint8_t>(value, ba::endianness::little);

        ASSERT_EQ(
            view.read<uint8_t>(0, ba::endianness::little),
            value
        );
    }
}

TEST(View, ReadUint32)
{
    const uint32_t value = 0xDEADBEEF;

    {
        ba::bytearray data{};

        ba::bytearray_view view(data, 0, 0);

        view.push_back<uint32_t>(value, ba::endianness::big);

        ASSERT_EQ(
            view.read<uint32_t>(0, ba::endianness::big),
            value
        );
    }

    {
        ba::bytearray data{};

        ba::bytearray_view view(data, 0, 0);

        view.push_back<uint32_t>(value, ba::endianness::little);

        ASSERT_EQ(
            view.read<uint32_t>(0, ba::endianness::little),
            value
        );
    }
}

TEST(View, ReadUint64)
{
    const uint64_t value = 0xDEADBEEFFFEEFFEE;

    {
        ba::bytearray data{};

        ba::bytearray_view view(data, 0, 0);

        view.push_back<uint64_t>(value, ba::endianness::big);

        ASSERT_EQ(
            view.read<uint64_t>(0, ba::endianness::big),
            value
        );
    }

    {
        ba::bytearray data{};

        ba::bytearray_view view(data, 0, 0);

        view.push_back<uint64_t>(value, ba::endianness::little);

        ASSERT_EQ(
            view.read<uint64_t>(0, ba::endianness::little),
            value
        );
    }
}

TEST(View, ReadInt64)
{
    const int64_t value = 0xDEADBEEFFFEEFFEE;

    {
        ba::bytearray data{};

        ba::bytearray_view view(data, 0, 0);

        view.push_back<int64_t>(value, ba::endianness::big);

        ASSERT_EQ(
            view.read<int64_t>(0, ba::endianness::big),
            value
        );
    }

    {
        ba::bytearray data{};

        ba::bytearray_view view(data, 0, 0);

        view.push_back<int64_t>(value, ba::endianness::little);

        ASSERT_EQ(
            view.read<int64_t>(0, ba::endianness::little),
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

TEST(View, ReadCustom)
{
    CustomStructDerived value = {};

    value.a = 0xDEADDEADBEEFBEEF;
    value.c = 0xFEEDBEEF;
    value.z = '%';
    memcpy(value.message, "Sample message", 15);
    memcpy(value.additional, "Some actual long additional message", 35);

    {
        ba::bytearray data{};

        ba::bytearray_view view(data, 0, 0);

        view.push_back(value, ba::endianness::big);

        ASSERT_TRUE(
            view.read<CustomStructDerived>(0, ba::endianness::big) == value
        );
    }

    {
        ba::bytearray data{};

        ba::bytearray_view view(data, 0, 0);

        view.push_back(value, ba::endianness::little);

        ASSERT_TRUE(
            view.read<CustomStructDerived>(0, ba::endianness::little) == value
        );
    }
}

TEST(View, ReadPart)
{
    uint64_t value = 0x00ADBEEFFFEEAABB;

    {
        ba::bytearray data{};

        ba::bytearray_view view(data, 0, 0);

        view.push_back_part(value, 7, ba::endianness::big);

        ASSERT_EQ(value, view.read_part<uint64_t>(0, 7, ba::endianness::big));
    }

    {
        ba::bytearray data{};

        ba::bytearray_view view(data, 0, 0);

        view.push_back_part(value, 7, ba::endianness::little);

        ASSERT_EQ(value, view.read_part<uint64_t>(0, 7, ba::endianness::little));
    }
}