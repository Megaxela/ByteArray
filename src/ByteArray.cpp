//
// Created by megaxela on 8/15/17.
//

#include "ByteArray.hpp"
#include <cstring>

ByteArray::ByteArray() :
        m_d(nullptr),
        m_n(0),
        m_c(0)
{

}

ByteArray::ByteArray(uint32_t capacity) :
        m_d(nullptr),
        m_n(0),
        m_c(0)
{
    changeCapacity(capacity);
}

ByteArray::ByteArray(const uint8_t *array, uint32_t size) :
        m_d(nullptr),
        m_n(size),
        m_c(size)
{
    if (size != 0)
    {
        m_d = new uint8_t[m_c];
        memcpy(m_d, array, sizeof(uint8_t) * m_n);
    }
}

ByteArray::ByteArray(const ByteArray &rhs) :
        m_d(nullptr),
        m_n(0),
        m_c(0)
{
    (*this) = rhs;
}

ByteArray::ByteArray(ByteArray&& rhs) noexcept :
    m_d(rhs.m_d),
    m_n(rhs.m_n),
    m_c(rhs.m_c)
{
    rhs.m_d = nullptr;
    rhs.m_n = 0;
    rhs.m_c = 0;
}

ByteArray::~ByteArray()
{
    delete []m_d;
}

void ByteArray::append(const ByteArray &array)
{
    append(array.m_d, array.m_n);
}

void ByteArray::append(const uint8_t *array, uint32_t size)
{
    if (m_n + size > m_c)
        changeCapacity(m_n + size);

    memcpy(m_d + m_n, array, size);
    m_n += size;
}

void ByteArray::changeCapacity(uint32_t t)
{
    m_c = t;
    auto *newData = new uint8_t[m_c];
    memset(newData, 0, sizeof(uint8_t) * m_c);

    if (m_n > 0 && m_d != nullptr)
    {
        memcpy(newData, m_d, sizeof(uint8_t) * m_n);
        delete[] m_d;
        m_d = nullptr;
    }

    m_d = newData;
}

uint32_t ByteArray::length() const
{
    return m_n;
}

uint32_t ByteArray::size() const
{
    return m_n;
}

uint32_t ByteArray::capacity() const
{
    return m_c;
}

uint8_t &ByteArray::operator [](unsigned int i)
{
    assert(i < m_n);

    return m_d[i];
}

uint8_t ByteArray::operator [](unsigned int i) const
{
    assert(i < m_n);

    return m_d[i];
}

std::ostream& operator <<(std::ostream &ostream, const ByteArray &arr)
{
    ostream << "ByteArray({" << std::endl;
    ostream << "               #-------------#-------------#-------------#-------------#" << std::endl;
    ostream << "               | 00 01 02 03 | 04 05 06 07 | 08 09 0A 0B | 0C 0D 0E 0F |" << std::endl;
    ostream << "               #-------------#-------------#-------------#-------------#";
    if (!arr.m_d)
    {
        ostream << "}, Length: " << arr.m_n << ", Capacity: " << std::dec << arr.m_c << ')' << std::endl;
        return ostream;
    }

    std::ios_base::fmtflags oldFlags = ostream.flags();
    std::streamsize         oldPrec  = ostream.precision();
    char                    oldFill  = ostream.fill();

    ostream.fill('0');

    unsigned int index = 0;
    for (index = 0; index < arr.m_n + (16 - (arr.m_n % 16)); ++index)
    {
        if (!(index % 16))
        {
            if (index)
                ostream << "| ";

            for (unsigned int asc = index - 16; asc < index; ++asc)
                if (arr.m_d[asc] >= ' ' && arr.m_d[asc] <= '~')
                    ostream << arr.m_d[asc];
                else
                    ostream << '.';

            ostream << std::endl << "    0x";
            ostream.width(8);
            ostream << std::hex << index << ' ';
        }

        if (!(index % 4))
            ostream << "| ";

        if (index < arr.m_n)
        {
            ostream.width(2);
            ostream << std::uppercase << std::hex << static_cast<unsigned int>(arr.m_d[index]) << ' ';
        }
        else
            ostream << "   ";
    }

    if (index)
        ostream << "| ";

    for (unsigned int asc = index - 16; asc < index; ++asc)
    {
        if (asc < arr.m_n)
        {
            if (arr.m_d[asc] >= ' ' && arr.m_d[asc] <= '~')
                ostream << arr.m_d[asc];
            else
                ostream << '.';
        }
        else
            ostream << ' ';
    }

    ostream << std::endl << std::nouppercase
            << "}, Length: " << std::dec << arr.m_n << ", Capacity: " << std::dec << arr.m_c << ')' << std::endl;

    ostream.flags(oldFlags);
    ostream.precision(oldPrec);
    ostream.fill(oldFill);

    return ostream;
}

const uint8_t *ByteArray::data() const
{
    return m_d;
}

ByteArray& ByteArray::operator=(ByteArray&& array) noexcept
{
    m_d = array.m_d;
    m_n = array.m_n;
    m_c = array.m_c;

    array.m_d = nullptr;
    array.m_n = 0;
    array.m_c = 0;

    return (*this);
}

ByteArray &ByteArray::operator =(const ByteArray &array)
{
    delete[] m_d;

    m_d = new uint8_t[array.m_n];
    memcpy(m_d, array.m_d, array.m_n);

    m_c = array.m_n;
    m_n = array.m_n;

    return (*this);
}

std::string ByteArray::toHex() const
{
    std::stringstream ss;

    for (uint32_t i = 0; i < m_n; ++i)
    {
        ss.width(2);
        ss.fill('0');
        ss << std::uppercase << std::hex << (int) m_d[i];
    }

    return ss.str();
}

bool ByteArray::operator==(const ByteArray &array) const
{
    if (m_n != array.m_n)
        return false;

    for (uint32_t i = 0; i < m_n; ++i)
        if (array.m_d[i] != m_d[i])
            return false;
    return true;
}

bool ByteArray::operator!=(const ByteArray &array) const
{
    return !((*this) == array);
}

ByteArray ByteArray::cut(unsigned int start, unsigned int end) const
{
    assert(start < end);
    assert(end <= length());

    return ByteArray(m_d + start, end - start);
}

void ByteArray::insert(uint32_t position, const ByteArray &array)
{
    assert(position < m_n);

    if (m_n + array.length() > m_c)
        changeCapacity(m_n + array.length());

    for (uint32_t index = m_n - 1; index >= position; --index)
        m_d[index + array.length()] = m_d[index];

    for (uint32_t index = 0; index < array.length(); ++index)
        m_d[index + position] = array.m_d[index];

    m_n += array.length();
}

void ByteArray::insert(uint32_t position, const uint8_t *array, uint32_t size)
{
    assert(position < m_n);

    if (m_n + size > m_c)
        changeCapacity(m_n + size);

    for (uint32_t index = m_n - 1; index >= position; --index)
        m_d[index + size] = m_d[index];

    for (uint32_t index = 0; index < size; ++index)
        m_d[index + position] = array[index];

    m_n += size;
}

bool ByteArray::empty() const
{
    return length() <= 0;
}

ByteArray ByteArray::reversed() const
{
    ByteArray copy = (*this);

    for (uint32_t i = 0; i < m_n; ++i)
        copy[i] = m_d[m_n - i - 1];

    return copy;
}

int char2int(char input)
{
    if(input >= '0' && input <= '9')
        return input - '0';
    if(input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if(input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    throw std::invalid_argument("Invalid input string");
}

// This function assumes include to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters, and target to be sufficiently large
void hex2bin(const char* src, ByteArray* arr)
{
    while(*src && src[1])
    {
        (*arr).append(static_cast<uint8_t>(
                              char2int(*src)*16 + char2int(src[1])
                      ));
        src += 2;
    }
}

ByteArray ByteArray::fromHex(const std::string &hex)
{
    int num = 0;
    for (char iter : hex)
    {
        if ((iter >= '0' && iter <= '9') ||
            (iter >= 'A' && iter <= 'F') ||
            (iter >= 'a' && iter <= 'f'))
        {
            ++num;
        }
    }

    if (num % 2)
    {
        return ByteArray();
    }

    ByteArray data;

    try
    {
        hex2bin(hex.c_str(), &data);
    }
    catch(std::invalid_argument &e)
    {
        return ByteArray();
    }

    return data;
}

ByteArray ByteArray::mid(uint32_t begin, uint32_t length) const
{
    return ByteArray(m_d + begin, length);
}

void ByteArray::clear()
{
    m_n = 0;
}

ByteArray ByteArray::fromASCII(const std::string& string)
{
    return ByteArray(reinterpret_cast<const uint8_t *>(string.c_str()),
                     static_cast<uint32_t>(string.size()));
}

void ByteArray::replace(uint32_t position, uint32_t size, const ByteArray& data)
{
    assert(position < m_n);
    assert(position + size <= m_n);

    if (size == 0)
    {
        return;
    }

    auto dataSize = data.size();
    auto difference = std::max(size, dataSize) - std::min(size, dataSize);

    if (m_n + difference > m_c)
    {
        // Have to reallocate memory
        changeCapacity(m_n + difference);
    }

    // Copying end if we need to
    if (position + size < m_n)
    {
        memcpy(m_d + (position + data.size()), m_d + (position + size), m_n - (position + size));
    }

    // Copying actual data to buffer
    memcpy(m_d + position, data.data(), data.size());

    if (size > data.size())
    {
        m_n -= difference;
    }
    else if (size < data.size())
    {
        m_n += difference;
    }
}
