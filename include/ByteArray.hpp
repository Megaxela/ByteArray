#pragma once

#include <cstdint>
#include <sstream>

#include <iostream>
#include <memory.h>
#include <cassert>

/**
 * @brief Class, that describes byte array.
 */
class ByteArray
{
    /**
     * @brief Stream output function.
     */
    friend std::ostream &operator<<(std::ostream &ostream, const ByteArray &arr);

public:
    /**
     * @brief Byte order enum.
     */
    enum ByteOrder
    {
        ByteOrder_BigEndian,    ///< Order in decreasing order (Big-endian)
        ByteOrder_LittleEndian  ///< Order in increasing order (Little-endian)
    };

    /**
     * @brief Default constructor.
     */
    ByteArray();

    /**
     * @brief Constructor with initial capacity.
     * @param capacity Initial capacity.
     */
    explicit ByteArray(uint32_t capacity);

    /**
     * @brief Constructor from byte array.
     * @param array Byte array.
     * @param size Byte array size.
     */
    ByteArray(const uint8_t* array, uint32_t size);

    /**
     * @brief Copy constructor.
     */
    ByteArray(const ByteArray &rhs);

    /**
     * @brief Move constructor.
     */
    ByteArray(ByteArray&& rhs) noexcept;

    /**
     * @brief Destructor.
     */
    ~ByteArray();

    /**
     * @brief Method for appending other byte array.
     * @param array Byte array.
     */
    void append(const ByteArray &array);

    /**
     * @brief Method for appending byte array.
     * @param array Byte array.
     * @param size Byte array size.
     */
    void append(const uint8_t* array, uint32_t size);

    template<typename T>
    void appendPart(T number, uint32_t size, ByteOrder order = ByteOrder_BigEndian)
    {
        assert(sizeof(T) >= size);

        if (m_n + size > m_c)
        {
            changeCapacity(m_n + size);
        }

        for (uint32_t i = 0;
             i < size;
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                m_d[m_n + i] = static_cast<uint8_t>(
//                    SystemTools::shr(number, i * 8)
                    number >> (i * 8)
                );
            }
            else
            {
                m_d[m_n + size - i - 1] = static_cast<uint8_t>(
//                    SystemTools::shr(number, i * 8)
                    number >> (i * 8)
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
    template<typename T>
    void appendMultiple(T number, uint32_t count, ByteOrder order = ByteOrder_BigEndian)
    {
        if (m_n + sizeof(T) * count > m_c)
        {
            changeCapacity(m_n + sizeof(T) * count);
        }

        for (int i = 0;
             i < static_cast<int>(count);
             ++i)
        {
            append(number, order);
        }
    }

    /**
     * @brief Template method for adding number to the end.
     */
    template<typename T>
    void append(T number, ByteOrder order = ByteOrder_BigEndian)
    {
        assert(order == ByteOrder_LittleEndian || order == ByteOrder_BigEndian);

        if (m_n + sizeof(T) > m_c)
        {
            changeCapacity(m_n + sizeof(T));
        }

        for (int i = 0;
             i < static_cast<int>(sizeof(T));
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                m_d[m_n + i] = static_cast<uint8_t>(
//                    SystemTools::shr<T>(number, i * 8)
                    number >> (i * 8)
                );
            }
            else
            {
                m_d[m_n + sizeof(T) - i - 1] = static_cast<uint8_t>(
//                    SystemTools::shr<T>(number, i * 8)
                    number >> (i * 8)
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
    void insert(uint32_t position, const ByteArray &array);

    /**
     * @brief Method for inserting byte array to specified position.
     * @param position Insertions position.
     * @param array Byte array.
     * @param size Byte array size.
     */
    void insert(uint32_t position, const uint8_t* array, uint32_t size);

    /**
     * @brief Method for inserting number into byte array.
     * @tparam T Inserting value type.
     * @param position Insertion position.
     * @param number Insertable number.
     * @param order Byte order.
     */
    template<typename T>
    void insert(uint32_t position, T number, ByteOrder order = ByteOrder_BigEndian)
    {
        assert(position < m_n);

        if (m_n + sizeof(T) > m_c)
        {
            changeCapacity(m_n + 4);
        }

        for (uint32_t index = m_n - 1; index >= position; --index)
        {
            m_d[index + sizeof(T)] = m_d[index];
        }

        for (int i = 0; i < sizeof(T); ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                m_d[position + i] = static_cast<uint8_t>(
//                    SystemTools::shr(number, i * 8)
                    number >> (i * 8)
                );
            }
            else
            {
                m_d[position + sizeof(T) - i - 1] = static_cast<uint8_t>(
//                    SystemTools::shr(number, i * 8)
                    number >> (i * 8)
                );
            }
        }

        m_n += sizeof(T);
    }

    template<typename T>
    void set(uint32_t position, T number, ByteOrder order = ByteOrder_BigEndian)
    {
        assert(position + (sizeof(T) - 1) < m_n);
        for (int i = 0;
             i < static_cast<int>(sizeof(T));
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                m_d[position + i] = static_cast<uint8_t>(
//                    SystemTools::shr(number, i * 8)
                    number >> (i * 8)
                );
            }
            else
            {
                m_d[position + sizeof(T) - i - 1] = static_cast<uint8_t>(
//                    SystemTools::shr(number, i * 8)
                    number >> (i * 8)
                );
            }
        }
    }

    void set(uint32_t position, const ByteArray& byteArray)
    {
        assert(position + byteArray.size() - 1 < m_n);

        for (uint32_t i = 0; i < byteArray.size(); ++i)
        {
            m_d[position + i] = byteArray[i];
        }
    }

    template<typename T>
    void setPart(uint32_t position, T number, uint32_t size, ByteOrder order = ByteOrder_BigEndian)
    {
        assert(sizeof(T) >= size);
        assert(position + (size - 1) < m_n);

        for (uint32_t i = 0;
             i < size;
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                m_d[position + i] = static_cast<uint8_t>(
//                    SystemTools::shr(number, i * 8)
                    number >> (i * 8)
                );
            }
            else
            {
                m_d[position + size - i - 1] = static_cast<uint8_t>(
//                    SystemTools::shr(number, i * 8)
                    number >> (i * 8)
                );
            }
        }
    }

    template<typename T>
    T read(uint32_t position, ByteOrder order = ByteOrder_BigEndian) const
    {
        assert(order == ByteOrder_LittleEndian || order == ByteOrder_BigEndian);
        assert((position + sizeof(T)) <= length());

        T value = T(0);

        for (int i = 0;
             i < static_cast<int>(sizeof(T));
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
//                value |= SystemTools::shl<T>(m_d[position + i], i * 8);
                value |= m_d[position + i] << (i * 8);
            }
            else
            {
//                value |= SystemTools::shl<T>(m_d[position + sizeof(T) - i - 1], i * 8);
                value |= m_d[position + sizeof(T) - i - 1] << (i * 8);
            }
        }

        return value;
    }

    template<typename T>
    T readPart(uint32_t position, uint8_t size, ByteOrder order = ByteOrder_BigEndian) const
    {
        assert(order == ByteOrder_LittleEndian || order == ByteOrder_BigEndian);
        assert((position + size) <= length());

        T value = T(0);

        for (int i = 0;
             i < static_cast<int>(size);
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
//                value |= SystemTools::shl<T>(m_d[position + i], i * 8);
                value |= m_d[position + i] << (i * 8);
            }
            else
            {
//                value |= SystemTools::shl<T>(m_d[position + size - i - 1], i * 8);
                value |= m_d[position + size - i - 1] << (i * 8);
            }
        }

        return value;
    }

    void replace(uint32_t position, uint32_t size, const ByteArray& data);

    static ByteArray fromHex(const std::string &hex);

    static ByteArray fromASCII(const std::string& string);

    uint32_t length() const;

    uint32_t size() const;

    bool empty() const;

    uint32_t capacity() const;

    const uint8_t* data() const;

    ByteArray reversed() const;

    void clear();

    ByteArray cut(unsigned int start, unsigned int end) const;

    void changeCapacity(uint32_t c);

    uint8_t& operator[](unsigned int i);

    uint8_t  operator[](unsigned int i) const;

    ByteArray &operator=(const ByteArray &array);

    ByteArray& operator=(ByteArray&& array) noexcept;

    std::string toHex() const;

    ByteArray mid(uint32_t begin, uint32_t length) const;

    bool operator==(const ByteArray &array) const;

    bool operator!=(const ByteArray &array) const;

private:

    /**
     * Pointer to data.
     */
    uint8_t *m_d;

    /**
     * Useful data size.
     */
    uint32_t m_n;

    /**
     * Container size (capacity).
     */
    uint32_t m_c;
};

