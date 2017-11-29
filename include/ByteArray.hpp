//
// Created by megaxela on 11/30/17.
//

#pragma once

#include <ostream>
#include <memory>
#include <cstring>
#include <mutex>

/**
 * @brief Class, that describes byte array.
 */
class ByteArray
{
    /**
     * @brief Stream output function.
     */
    friend std::ostream &operator<<(std::ostream &ostrea, const ByteArray &arr);

    struct Impl;

    using ImplPtr = std::shared_ptr<Impl>;

    struct Impl
    {
        Impl() :
                m_curr(nullptr),
                m_currSize(nullptr),
                m_n(0),
                m_c(0),
                m_d(nullptr),
                m_mutex()
        {

        }

        ~Impl()
        {
            delete[] m_d;
        }

        ImplPtr copy()
        {
            auto result = std::make_shared<Impl>();

            result->m_n = m_n;
            result->m_c = m_c;
            result->m_d = new uint8_t[m_c];

            std::memcpy(m_d, result->m_d, sizeof(uint8_t) * m_n);

            return result;
        }

        static ImplPtr create()
        {
            return std::make_shared<Impl>();
        }

        /**
         * Logic pointer to data. It can be
         * different from m_d field, when
         * ByteArray::cut was applied.
         * It used for preventing useless copying.
         */
        uint8_t* m_curr;

        /**
         * Logic size. It can be different
         * from m_n field, when ByteArray::cut
         * was applied as well.
         */
        uint32_t m_currSize;

        /**
         * Useful data size for m_d;
         */
        uint32_t m_n;

        /**
         * Container size (capacity).
         */
        uint32_t m_c;

        /**
         * @brief Pointer to container.
         */
        uint8_t* m_d;

        /**
         * @brief Multithread safe.
         */
        std::mutex m_mutex;
    };

public:

    using size_t = uint32_t;
    using byte = uint8_t;
    using value_type = byte;

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
            m_impl(nullptr)
    {

    }

    /**
     * @brief Constructor with initial capacity.
     * @param capacity Initial capacity.
     */
    explicit ByteArray(size_t capacity) :
            m_impl(nullptr)
    {
        changeCapacity(capacity);
    }

    /**
     * @brief Constructor from byte array.
     * @param array Byte array.
     * @param size Byte array size.
     */
    ByteArray(const byte* array, size_t size) :
            m_impl(nullptr)
    {
        if (size > 0)
        {
            m_impl = Impl::create();

            std::lock_guard<std::mutex> lock(m_impl->m_mutex);
            m_impl->m_d = new uint8_t[size];
            memcpy(m_impl->m_d, array, sizeof(byte) * size);
        }
    }

    /**
     * @brief Copy constructor.
     */
    ByteArray(const ByteArray& rhs) :
            m_impl(rhs.m_impl)
    {

    }

    /**
     * @brief Move constructor.
     */
    ByteArray(ByteArray&& rhs) noexcept :
            m_impl(rhs.m_impl)
    {
        rhs.m_impl = nullptr;
    }

    /**
     * @brief Destructor.
     */
    ~ByteArray()
    {
        m_impl = nullptr;
    }

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
    void appendPart(T number, uint32_t size, ByteOrder order = ByteOrder_BigEndian);

    /**
     * @brief Template method for adding several
     * values.
     * @param number Value to be added of type T
     * @param count Number of added values.
     * @param order Byte order.
     */
    template<typename T>
    void appendMultiple(T number, uint32_t count, ByteOrder order = ByteOrder_BigEndian);

    /**
     * @brief Template method for adding number to the end.
     */
    template<typename T>
    void append(T number, ByteOrder order = ByteOrder_BigEndian);

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
    void insert(uint32_t position, T number, ByteOrder order = ByteOrder_BigEndian);

    template<typename T>
    void set(uint32_t position, T number, ByteOrder order = ByteOrder_BigEndian);

    void set(uint32_t position, const ByteArray& byteArray);

    template<typename T>
    void setPart(uint32_t position, T number, uint32_t size, ByteOrder order = ByteOrder_BigEndian);

    template<typename T>
    T read(uint32_t position, ByteOrder order = ByteOrder_BigEndian) const;

    template<typename T>
    T readPart(uint32_t position, uint8_t size, ByteOrder order = ByteOrder_BigEndian) const;

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

    ImplPtr m_impl;
};



