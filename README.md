# Byte Array
It's C++11 based simple byte array class, implemented with PIMPL.

## Build
It's header only library, so you may only to include ByteArray.hpp to your 
project.

You also can use it as CMake submodule. Everything you need is just to
add this project as submodule and link it to target.

### Build tests and benchmarks
If you want to build tests and benchmarks, you have to:
1. Load submodules with `git submodule init && git submodule update`.
1. Pass `-DBYTEARRAY_BUILD_TESTS=On` to `cmake` on configuration step. (`cmake -DBYTEARRAY_BUILD_TESTS=On .`)
1. Build: `cmake --build . -- -j2`

## Usage example
ByteArray mostly used for creating and parsing byte arrays.

Creating byte array example:
```cpp
#include <ByteArray.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    // Setting initial capacity for optimizations
    ByteArray byteArray(32 /* bytes */);

    byteArray.append<uint32_t>(0xDEADBEEF, ByteArray::ByteOrder_BigEndian);
    byteArray.append<uint32_t>(0xDEADBEEF, ByteArray::ByteOrder_LittleEndian);
    byteArray.append<uint64_t>(0xAABBCCDDDDCCBBAA);        // Big Endian - by default.
    byteArray.appendPart<uint64_t>(0x1122334455667788, 5); // Big Endian - by default.
    byteArray.appendMultiple<uint8_t>(0x00, 11);           // Big Endian - by default.

    // Result: 0xDEADBEEFEFBEADDEAABBCCDDDDCCBBAA44556677880000000000000000000000
    std::cout << "Current value: 0x" << byteArray.toHex() << std::endl;

    std::cout
        << "4 bytes == 0xDEADBEEF: "
        << std::boolalpha << (byteArray.read<uint32_t>(0, ByteArray::ByteOrder_BigEndian) == 0xDEADBEEF)
        << std::endl;

    std::cout
        << "4 bytes == 0xDEADBEEF: "
        << std::boolalpha << (byteArray.read<uint32_t>(4, ByteArray::ByteOrder_LittleEndian) == 0xDEADBEEF)
        << std::endl;

    std::cout
        << "8 bytes == 0xAABBCCDDDDCCBBAA: "
        << std::boolalpha << (byteArray.read<uint64_t>(8, ByteArray::ByteOrder_BigEndian) == 0xAABBCCDDDDCCBBAA)
        << std::endl;

    std::cout
        << "5 bytes == 0x4455667788: "
        << std::boolalpha << (byteArray.readPart<uint64_t>(16, 5) == 0x4455667788)
        << std::endl;

    // Debug byte array info.
    std::cout << byteArray << std::endl;

    // Result:
    // ByteArray({
    //                #-------------#-------------#-------------#-------------#
    //                | 00 01 02 03 | 04 05 06 07 | 08 09 0A 0B | 0C 0D 0E 0F |
    //                #-------------#-------------#-------------#-------------#
    //     0x00000000 | DE AD BE EF | EF BE AD DE | AA BB CC DD | DD CC BB AA | ................
    //     0x00000010 | 44 55 66 77 | 88 00 00 00 | 00 00 00 00 | 00 00 00 00 | DUfw............
    //     0x00000020 |             |             |             |             |                 
    // }, Length: 32, Capacity: 32)

    return 0;
}
```