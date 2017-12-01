//
// Created by megaxela on 11/30/17.
//

#pragma once

#include <ostream>
#include <memory>
#include <cstring>
#include <mutex>
#include <algorithm>
#include <sstream>

/**
 * @brief Class, that describes byte array.
 * Implemented through PIMPL to prevent
 * excess copies.
 */
class ByteArray
{
public:

    using size_type = uint32_t;
    using byte = uint8_t;
    using value_type = byte;

private:

    /**
     * @brief Stream output function.
     */
    friend std::ostream &operator<<(std::ostream &ostream, const ByteArray &arr)
    {
        ostream << "ByteArray({" << std::endl;
        ostream << "               #-------------#-------------#-------------#-------------#" << std::endl;
        ostream << "               | 00 01 02 03 | 04 05 06 07 | 08 09 0A 0B | 0C 0D 0E 0F |" << std::endl;
        ostream << "               #-------------#-------------#-------------#-------------#";
        if (!arr.m_curr || arr.m_impl == nullptr)
        {
            ostream << "}, Length: 0, Capacity: 0)" << std::endl;
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
                    if (arr.m_curr[asc] >= ' ' && arr.m_curr[asc] <= '~')
                        ostream << arr.m_curr[asc];
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
                ostream << std::uppercase << std::hex << static_cast<unsigned int>(arr.m_curr[index]) << ' ';
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
                if (arr.m_curr[asc] >= ' ' && arr.m_curr[asc] <= '~')
                    ostream << arr.m_curr[asc];
                else
                    ostream << '.';
            }
            else
                ostream << ' ';
        }

        ostream << std::endl << std::nouppercase
                << "}, Length: "
                << std::dec << arr.m_n
                << ", Capacity: "
                << std::dec << arr.m_impl->m_c
                << ')' << std::endl;

        ostream.flags(oldFlags);
        ostream.precision(oldPrec);
        ostream.fill(oldFill);

        return ostream;
    }

    struct Impl;

    using ImplPtr = std::shared_ptr<Impl>;

    struct Impl
    {
        Impl() :
                m_c(0),
                m_d(nullptr)
        {

        }

        Impl(const Impl&) = delete;
        Impl& operator=(const Impl&) = delete;

        ~Impl()
        {
            delete[] m_d;
        }

        static ImplPtr create()
        {
            return std::make_shared<Impl>();
        }

        /**
         * Container size (capacity).
         */
        size_type m_c;

        /**
         * @brief Pointer to container.
         */
        byte* m_d;
    };

public:

    /**
     * @brief Byte order enum.
     */
    enum ByteOrder
    {
        ByteOrder_BigEndian,   ///< Order in decreasing order (Big-endian)
        ByteOrder_LittleEndian ///< Order in increasing order (Little-endian)
    };

    /**
     * @brief Default constructor.
     */
    ByteArray() :
            m_impl(nullptr),
            m_curr(nullptr),
            m_n(0)
    {

    }

    /**
     * @brief Constructor with initial capacity.
     * @param capacity Initial capacity.
     */
    explicit ByteArray(size_type capacity) :
            m_impl(nullptr),
            m_curr(nullptr),
            m_n(0)
    {
        prepareImpl(capacity);
    }

    /**
     * @brief Constructor from byte array.
     * @param array Byte array.
     * @param size Byte array size.
     */
    ByteArray(const byte* array, size_type size) :
            m_impl(nullptr),
            m_curr(nullptr),
            m_n(0)
    {
        if (size > 0)
        {
            prepareImpl(size);
            memcpy(m_curr, array, sizeof(byte) * size);
            m_n = size;
        }
    }

    /**
     * @brief Copy constructor.
     */
    ByteArray(const ByteArray& rhs) :
            m_impl(rhs.m_impl),
            m_curr(rhs.m_curr),
            m_n(rhs.m_n)
    {

    }

    /**
     * @brief Move constructor.
     */
    ByteArray(ByteArray&& rhs) noexcept :
            m_impl(rhs.m_impl),
            m_curr(rhs.m_curr),
            m_n(rhs.m_n)
    {
        rhs.m_impl = nullptr;
    }

    /**
     * @brief Destructor.
     */
    ~ByteArray()
    {
        m_impl = nullptr;
        m_curr = nullptr;
    }

    /**
     * @brief Method for appending other byte array.
     * @param array Byte array.
     */
    void append(const ByteArray &array)
    {
        if (array.m_impl == nullptr)
        {
            return;
        }

        append(array.m_curr, array.m_n);
    }

    /**
     * @brief Method for appending byte array.
     * @param array Byte array.
     * @param size Byte array size.
     */
    void append(const byte* array, size_type size)
    {
        if (m_impl)
        {
            prepareImpl(m_n + size);
        }
        else
        {
            prepareImpl(size);
        }

        memcpy(m_curr + m_n, array, size);
        m_n += size;
    }

    template<typename T>
    void appendPart(T number,
                    size_type size,
                    ByteOrder order = ByteOrder_BigEndian)
    {
        if (sizeof(T) < size)
        {
            throw std::invalid_argument("Can't append more bytes than type has.");
        }

        if (m_impl)
        {
            prepareImpl(m_n + size);
        }
        else
        {
            prepareImpl(size);
        }

        for (size_type i = 0;
             i < size;
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                m_curr[m_n + i] = static_cast<uint8_t>(
                    shr(number, (i * 8))
                );
            }
            else
            {
                m_curr[m_n + size - i - 1] = static_cast<uint8_t>(
                    shr(number, (i * 8))
                );
            }
        }

        m_n += size;
    }

    /**
     * @brief Template method for adding several
     * values.
     * @param number Value to be added of type T
     * @param count Number of added values.
     * @param order Byte order.
     */
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    void appendMultiple(T number, size_type count, ByteOrder order = ByteOrder_BigEndian)
    {
        if (m_impl)
        {
            prepareImpl(m_n + sizeof(T) * count);
        }
        else
        {
            prepareImpl(sizeof(T) * count);
        }

        for (size_type i = 0;
             i < count;
             ++i)
        {
            append(number, order);
        }
    }

    /**
     * @brief Template method for adding number to the end.
     */
    template<
        typename T,
        typename = typename std::enable_if<std::is_arithmetic<T>::value>::type
    >
    void append(T number, size_type order = ByteOrder_BigEndian)
    {
        if (m_impl)
        {
            prepareImpl(m_n + sizeof(T));
        }
        else
        {
            prepareImpl(sizeof(T));
        }

        for (size_type i = 0;
             i < sizeof(T);
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                m_curr[m_n + i] = static_cast<uint8_t>(
                    shr(number, i * 8)
                );
            }
            else
            {
                m_curr[m_n + sizeof(T) - i - 1] = static_cast<uint8_t>(
                    shr(number, i * 8)
                );
            }
        }

        m_n += sizeof(T);
    }

    /**
     * @brief Method for inserting byte array to specified position.
     * @param position Insertions position.
     * @param array Byte array.
     */
    void insert(size_type position, const ByteArray &array)
    {
        if (array.m_impl == nullptr)
        {
            return;
        }

        if (m_impl)
        {
            if (position >= m_n)
            {
                throw std::out_of_range("Insert position is out of range.");
            }

            prepareImpl(m_n + array.length());
        }
        else
        {
            if (position > 0)
            {
                throw std::out_of_range("Insert position is out of range.");
            }

            prepareImpl(array.length());
        }

        if (m_n > 0)
        {
            for (size_type index = m_n - 1; index >= position; --index)
            {
                m_curr[index + array.length()] = m_curr[index];
            }
        }

        for (size_type index = 0; index < array.length(); ++index)
        {
            m_curr[index + position] = array.m_curr[index];
        }

        m_n += array.length();
    }

    /**
     * @brief Method for inserting byte array to specified position.
     * @param position Insertions position.
     * @param array Byte array.
     * @param size Byte array size.
     */
    void insert(size_type position, const byte* array, size_type size)
    {
        if (m_impl)
        {
            if (position >= m_n)
            {
                throw std::out_of_range("Insert position is out of range.");
            }

            prepareImpl(m_n + size);
        }
        else
        {
            if (position > 0)
            {
                throw std::out_of_range("Insert position is out of range.");
            }

            prepareImpl(size);
        }

        if (m_n > 0)
        {
            for (size_type index = m_n - 1; index >= position; --index)
            {
                m_curr[index + size] = m_curr[index];
            }
        }

        for (size_type index = 0; index < size; ++index)
        {
            m_curr[index + position] = array[index];
        }

        m_n += size;
    }

    /**
     * @brief Method for inserting number into byte array.
     * @tparam T Inserting value type.
     * @param position Insertion position.
     * @param number Insertable number.
     * @param order Byte order.
     */
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    void insert(size_type position, T number, ByteOrder order = ByteOrder_BigEndian)
    {
        if (m_impl)
        {
            prepareImpl(m_n + sizeof(T));
        }
        else
        {
            prepareImpl(sizeof(T));
        }

        if (m_n > 0)
        {
            for (uint32_t index = m_n - 1; index >= position; --index)
            {
                m_curr[index + sizeof(T)] = m_curr[index];
            }
        }

        for (size_type i = 0;
             i < sizeof(T);
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                m_curr[position + i] = static_cast<uint8_t>(
                    shr(number, i * 8)
                );
            }
            else
            {
                m_curr[position + sizeof(T) - i - 1] = static_cast<uint8_t>(
                    shr(number, i * 8)
                );
            }
        }

        m_n += sizeof(T);
    }

    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    void set(size_type position, T number, ByteOrder order = ByteOrder_BigEndian)
    {
        if (m_impl)
        {
            if (position + (sizeof(T) - 1) >= m_n)
            {
                throw std::out_of_range("Position + type size is out of range.");
            }
        }
        else
        {
            throw std::out_of_range("Position + type size is out of range.");
        }

        prepareImpl(0);

        for (size_type i = 0;
             i < static_cast<int>(sizeof(T));
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                m_curr[position + i] = static_cast<uint8_t>(
                    shr(number, i * 8)
                );
            }
            else
            {
                m_curr[position + sizeof(T) - i - 1] = static_cast<uint8_t>(
                    shr(number, i * 8)
                );
            }
        }
    }

    void set(size_type position, const ByteArray& byteArray)
    {
        if (byteArray.m_impl == nullptr ||
            byteArray.size() == 0)
        {
            return;
        }

        if (m_impl)
        {
            if (position + byteArray.size() - 1 >= m_n)
            {
                throw std::out_of_range("Position + array size is out of range.");
            }
        }
        else
        {
            throw std::out_of_range("Position + array size is out of range.");
        }

        prepareImpl(0);

        for (size_type i = 0; i < byteArray.size(); ++i)
        {
            m_curr[position + i] = byteArray[i];
        }
    }

    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    void setPart(size_type position, T number, size_type size, ByteOrder order = ByteOrder_BigEndian)
    {
        if (size == 0)
        {
            return;
        }

        if (sizeof(T) < size)
        {
            throw std::invalid_argument("Can't set more bytes than type has.");
        }

        if (m_impl)
        {
            if (position + (size - 1) <= m_n)
            {
                throw std::out_of_range("Position + size is out of range.");
            }
        }
        else
        {
            throw std::out_of_range("Position + size is out of range.");
        }

        prepareImpl(0);

        for (size_type i = 0;
             i < size;
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                m_curr[position + i] = static_cast<uint8_t>(
                    shr(number, i * 8)
                );
            }
            else
            {
                m_curr[position + size - i - 1] = static_cast<uint8_t>(
                    shr(number, i * 8)
                );
            }
        }
    }

    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    T read(size_type position, ByteOrder order = ByteOrder_BigEndian) const
    {
        if (m_impl)
        {
            if (position + sizeof(T) > m_n)
            {
                throw std::out_of_range("Type is out of range.");
            }
        }
        else
        {
            throw std::out_of_range("Type is out of range.");
        }

        T value = T(0);

        for (size_type i = 0;
             i < static_cast<int>(sizeof(T));
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                value |= shl<T>(m_curr[position + i], static_cast<uint8_t>(i * 8));
            }
            else
            {
                value |= shl<T>(m_curr[position + sizeof(T) - i - 1], static_cast<uint8_t>(i * 8));
            }
        }

        return value;
    }

    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    T readPart(size_type position, uint8_t size, ByteOrder order = ByteOrder_BigEndian) const
    {
        if (m_impl)
        {
            if((position + size) > m_n)
            {
                throw std::out_of_range("Type is out of range.");
            }
        }
        else
        {
            throw std::out_of_range("Type is out of range.");
        }

        T value = T(0);

        for (size_type i = 0;
             i < size;
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                value |= shl<T>(m_curr[position + i], static_cast<uint8_t>(i * 8));
            }
            else
            {
                value |= shl<T>(m_curr[position + size - i - 1], static_cast<uint8_t>(i * 8));
            }
        }

        return value;
    }

    void replace(size_type position, size_type size, const ByteArray& data)
    {
        if (size == 0)
        {
            return;
        }

        auto dataSize = data.size();
        auto difference = std::max(size, dataSize) - std::min(size, dataSize);

        if (m_impl)
        {
            if (position >= m_n)
            {
                throw std::out_of_range("Position is bigger than amount of elements.");
            }

            if (position + size >= m_n)
            {
                throw std::out_of_range("End position is bigger than amount of elements.");
            }

            if (size < data.size())
            {
                prepareImpl(m_n + difference);
            }
            else
            {
                prepareImpl(m_n - difference);
            }
        }
        else
        {
            throw std::out_of_range("Position is bigger than amount of elements.");
        }

        // Copying end if we need to
        if (position + size < m_n)
        {
            std::memcpy(
                m_curr + (position + data.size()),
                m_curr + (position + size),
                (m_n - (position + size)) * sizeof(byte)
            );
        }

        // Copying actual data to buffer
        std::memcpy(
            m_curr + position,
            data.data(),
            data.size()
        );

        if (size > data.size())
        {
            m_n -= difference;
        }
        else if (size < data.size())
        {
            m_n += difference;
        }
    }

    static ByteArray fromHex(const std::string &hex, bool isStrict=true)
    {
        auto count = hex.size();

        static auto validator = [](char c)
        {
            return (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z') ||
                   (c >= '0' && c <= '9');
        };

        static auto char2int = [](char symbol) -> uint8_t
        {
            if(symbol >= '0' && symbol <= '9')
                return static_cast<uint8_t>(symbol - '0');
            else if(symbol >= 'A' && symbol <= 'F')
                return static_cast<uint8_t>(symbol - 'A' + 10);
            else if(symbol >= 'a' && symbol <= 'f')
                return static_cast<uint8_t>(symbol - 'a' + 10);

            throw std::invalid_argument(std::string("Symbol \"") + symbol + "\" is not hex.");
        };

        if (!isStrict)
        {
            count = static_cast<unsigned long>(
                std::count_if(
                    hex.begin(),
                    hex.end(),
                    validator
                )
            );
        }
        else
        {
            auto found = std::find_if(
                hex.begin(),
                hex.end(),
                [](char c)
                {
                    return (c < 'a' || c > 'z') &&
                           (c < 'A' || c > 'Z') &&
                           (c < '0' || c > '9');
                }
            );

            if (found != hex.end())
            {
                throw std::invalid_argument("Strict hex check failed.");
            }
        }

        if (count % 2 != 0)
        {
            throw std::invalid_argument("Number of symbols has to be % 2");
        }

        ByteArray result(static_cast<size_type>(count / 2));

        bool isFirst = true;
        uint8_t firstValue = 0;

        for (char symbol : hex)
        {
            if (!validator(symbol))
            {
                continue;
            }

            if (isFirst)
            {
                firstValue = char2int(symbol);

                isFirst = false;
            }
            else
            {
                result.append<uint8_t>(
                    static_cast<uint8_t>(firstValue * 16 + char2int(symbol))
                );

                isFirst = true;
            }

        }

        return result;
    }

    static ByteArray fromASCII(const std::string& string)
    {
        return ByteArray(
            reinterpret_cast<const byte *>(string.c_str()),
            static_cast<size_type>(string.size())
        );
    }

    size_type length() const
    {
        return size();
    }

    size_type size() const
    {
        if (m_impl == nullptr)
        {
            return 0;
        }

        return m_n;
    }

    bool empty() const
    {
        if (m_impl == nullptr)
        {
            return true;
        }

        return m_n == 0;
    }

    size_type capacity() const
    {
        if (m_impl == nullptr)
        {
            return 0;
        }

        return static_cast<size_type>(m_impl->m_c - (m_curr - m_impl->m_d));
    }

    const byte* data() const
    {
        if (m_impl == nullptr)
        {
            return nullptr;
        }

        return m_curr;
    }

    ByteArray reversed() const
    {
        if (m_impl == nullptr)
        {
            return ByteArray();
        }

        if (m_n == 0)
        {
            return ByteArray();
        }

        ByteArray byteArray(m_n);

        for (uint32_t i = 0; i < m_n; ++i)
            byteArray.append<uint8_t>(m_curr[m_n - i - 1]);

        return byteArray;
    }

    void clear()
    {
        prepareImpl(0);

        m_n = 0;
        m_curr = m_impl->m_d;
    }

    ByteArray cut(size_type start, size_type end) const
    {
        if (start > end)
        {
            throw std::invalid_argument("Start is smaller than end,");
        }

        ByteArray result;

        result.m_impl = m_impl;
        result.m_curr = m_curr + start;
        result.m_n = end - start;

        return result;
    }

    uint8_t& operator[](size_type i)
    {
        if (i > m_n)
        {
            throw std::out_of_range("Index is out of range.");
        }

        prepareImpl(0);

        return m_curr[i];
    }

    uint8_t operator[](size_type i) const
    {
        if (i > m_n)
        {
            throw std::out_of_range("Index is out of range.");
        }

        return m_curr[i];
    }

    ByteArray &operator=(const ByteArray &array)
    {
        m_impl = array.m_impl;
        m_n = array.m_n;
        m_curr = array.m_curr;

        return (*this);
    }

    ByteArray& operator=(ByteArray&& array) noexcept
    {
        m_impl = array.m_impl;
        m_n = array.m_n;
        m_curr = array.m_curr;

        array.m_impl = nullptr;

        return (*this);
    }

    std::string toHex() const
    {
        std::stringstream ss;

        for (uint32_t i = 0; i < m_n; ++i)
        {
            ss.width(2);
            ss.fill('0');
            ss << std::uppercase << std::hex << (int) m_curr[i];
        }

        return ss.str();
    }

    ByteArray mid(size_type begin, size_type length) const
    {
        if (begin + length > m_n)
        {
            throw std::invalid_argument("End is too far.");
        }

        ByteArray result;

        result.m_impl = m_impl;
        result.m_curr = m_curr + begin;
        result.m_n = length;

        return result;
    }

    bool operator==(const ByteArray &array) const
    {
        if (m_n != array.m_n)
        {
            return false;
        }

        if (m_impl == array.m_impl &&
            m_curr == array.m_curr)
        {
            return true;
        }

        for (size_type i = 0; i < m_n; ++i)
        {
            if (array.m_curr[i] != m_curr[i])
            {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const ByteArray &array) const
    {
        return !((*this) == array);
    }

private:

    void prepareImpl(size_type size)
    {
        if (m_impl == nullptr)
        {
            m_impl = Impl::create();

            m_impl->m_d = new uint8_t[size];
            m_impl->m_c = size;

            m_curr = m_impl->m_d;

            m_n = 0;
        }
        else if (m_impl.use_count() > 1)
        {
            auto newImpl = Impl::create();

            auto newCap = m_n > size ? m_n : size;

            newImpl->m_d = new uint8_t[newCap];
            newImpl->m_c = newCap;

            std::memcpy(
                newImpl->m_d,
                m_curr,
                m_n * sizeof(byte)
            );

            m_curr = newImpl->m_d;
            m_impl = newImpl;
        }

        if (size > m_impl->m_c - (m_curr - m_impl->m_d))
        {
            auto newData = new uint8_t[size];

            std::memcpy(
                newData,
                m_curr,
                m_n
            );

            delete[] m_impl->m_d;

            m_impl->m_d = newData;
            m_impl->m_c = size;
            m_curr = m_impl->m_d;
        }
    }

    /**
     * @brief Right shift for any PC. Because of not any PC can shift more than 31 bit.
     * @tparam T Type of shifted variable.
     * @tparam IndexType Type of step.
     * @tparam Step Maximum step. (Hardware based)
     * @param v Variable to shift.
     * @param n Value of shift.
     * @return Shifted value.
     */
    template<typename T, typename IndexType=uint8_t, IndexType Step=31 >
    static T shr(T v, IndexType n)
    {
        for (IndexType i = 0; i < n; i += std::min(static_cast<IndexType>(n - i), static_cast<IndexType>(Step)))
        {
            v >>= std::min(static_cast<IndexType>(n - i), static_cast<IndexType>(Step));
        }

        return v;
    }

    /**
     * @brief Left shift for any PC. Because of not any PC can shift more than 31 bit.
     * @tparam T Type of shifted variable.
     * @tparam IndexType Type of step.
     * @tparam Step Maximum step. (Hardware based)
     * @param v Variable to shift.
     * @param n Value of shift.
     * @return Shifted value.
     */
    template<typename T, typename IndexType=uint8_t, IndexType Step=31 >
    static T shl(T v, IndexType n)
    {
        for (IndexType i = 0; i < n; i += std::min(static_cast<IndexType>(n - i), static_cast<IndexType>(Step)))
        {
            v <<= std::min(static_cast<IndexType>(n - i), static_cast<IndexType>(Step));
        }

        return v;
    }

    ImplPtr m_impl;

    /**
     * Logic pointer to data. It can be
     * different from m_d field, when
     * ByteArray::cut was applied.
     * It used for preventing useless copying.
     */
    byte* m_curr;

    /**
     * Useful data size for m_d;
     */
    size_type m_n;
};



