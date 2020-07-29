# Byte Array
It's C++17 STL based set of classes that describes byte array.

## Structure
Library contains 3 base classes (See Examples for samples):
* `bytearray_processor` - it's byte array interface with all methods 
implementation. It does not contains actual data. 
* `bytearray` - it's class, that owns data and has `bytearray_processor` 
interface.
* `bytearray_view` - class, that describes object that can refer to some 
part of `bytearray`. It's also has `bytearray_processor` interface, but
non constant methods will change parent `bytearray`.

## Build
It's header only library, so you may only include headers from `include` 
directory.

You also can use it as CMake submodule. Everything you need is just to add 
this project as submodule and link it to target.

### Build tests and benchmarks
1. Load submodules with `git submodule init && git submodule update`.
1. Pass `BYTEARRAY_BUILD_TESTS` to `cmake` on configuration step (`cmake -DBYTEARRAY_BUILD_TESTS=On`)
1. Build `cmake --build . -- -j2`

## Examples

Usage of `bytearray_processor`:
```cpp
#include <ba/bytearray_processor.hpp>

int main(int argc, char** argv)
{
    // You may use any numerical type, that has 1 byte size.
    std::vector<std::byte> some_vector;
    
    ba::bytearray_processor processor(some_vector);
    
    processor.push_back<uint32_t>(0xDEADBEEF, ba::endianness::little);
    
    /* Output:
     * ByteArray({
     *               #-------------#-------------#-------------#-------------#
     *               | 00 01 02 03 | 04 05 06 07 | 08 09 0A 0B | 0C 0D 0E 0F |
     *               #-------------#-------------#-------------#-------------#
     *    0x00000000 | EF BE AD DE |             |             |             | ....            
     *               #-------------#-------------#-------------#-------------#
     *}, Length: 4, Capacity: 4)
     */
    std::cout << processor << std::endl;
    
    /* Output:
     * EFBEADDE
     */
    std::cout << std::to_string(processor) << std::endl;
    
    /* Output:
     * true
     */
    std::cout 
        << std::boolalpha 
        << (processor.read<uint32_t>(0, ba::endianness::little) == 0xDEADBEEF) 
        << std::endl;
        
    /* Output:
     * 4
     */
    std::cout << some_vector.size() << std::endl;
}

```

You may replace
```cpp
std::vector<std::byte> some_vector;
ba::bytearray_processor processor(some_vector);
```
with `ba::bytearray`. You will be able to access vector with `container` method.

Usage of `ba::bytearray_view`:

```cpp
#include <ba/bytearray.hpp>
#include <ba/bytearray_view.hpp>

int main()
{
    // It's equal to (this suffix is not `constexpr`):
    // bytearray array{};
    // array.load_from_hex("DEADBEEF");
    auto array = "DEADBEEF"_ba;
    
    ba::bytearray_view view(array, 1, 2);
    
    /* Output:
     * ADBE
     */ 
    std::cout << std::to_string(view) << std::endl;

    view.push_back<uint16_t>(0xAAFF); // Big endian by default
    
    /* Output:
     * ADBEAAFF
     */
    std::cout << std::to_string(view) << std::endl;
    
    /* Output:
     * DEADBEAAFFEF
     */
    std::cout << std::to_string(array) << std::endl;
}
```

## License
<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

Library is licensed under the [MIT License](https://opensource.org/licenses/MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.