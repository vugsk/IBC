//
// Created by ganch on 25.11.2024.
//

#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>

#include "ErrorInfo.hpp"
#include "GeneralFunction.hpp"


namespace chunks {

static constexpr uint8_t DEFAULT_SIZE_BLOCK = 4;


class func_check_string_in_int_
{
public:
    constexpr func_check_string_in_int_() = default;
    constexpr ~func_check_string_in_int_() = default;

    template<typename T>
    constexpr bool operator()(const T& str) const
    {
        auto num = 0;
        for (const auto& i : str)
            if (isdigit(i))
                ++num;
        return static_cast<size_t>(num) == str.size();
    }
};

static inline constexpr func_check_string_in_int_ checkStringInInt{};



enum class ModeChunk : uint8_t
{
    NONE, // default mode
    READ,
    WRITE,
    MIXED, // maybe I will
};

class Chunk
{
    static constexpr uint64_t val1 = 0xEDB88320UL; // rename
    static constexpr uint64_t max_uint64 = 0xFFFFFFFFUL; // rename

    char _name[DEFAULT_SIZE_BLOCK];
    uint32_t _begin;
    uint32_t _end;
    uint32_t _size;
    uint32_t _crc;
    uint16_t _length;

    ModeChunk _mode;

    bool _isCalcLength;
    bool _isCalcTypeChunk;
    bool _isCalcCrc;

    constexpr auto convertFromFromArrayInCrc(const char* const& crc) -> decltype(_crc)
    {
        errorsChunk<ErrorsChunks::CAN_NOT_REDEFINE_CRC_BLOCK>(
            std::forward<bool>(_isCalcCrc));
        _isCalcCrc = true;
        return convertStringInInt<DEFAULT_SIZE_BLOCK, decltype(_crc), 16>(crc);
    }

    static constexpr uint32_t calcCrc(const char*&& buf, uint64_t&& len)
    {
        uint64_t crc_table[256];
        uint64_t crc;
        for (uint16_t i = 0; i < 256; i++)
        {
            crc = i;
            for (uint8_t j = 0; j < 8; j++)
                crc = (crc >> 1) ^ (crc & 1 ? val1 : 0);
            crc_table[i] = crc;
        };

        crc = max_uint64;
        while (len--)
            crc = (crc >> 8) ^ crc_table[(crc ^ *buf++) & 0xFF];
        return crc ^ max_uint64;
    }

protected:
    bool _isCanMultiChucks;
    const char* _data;

    template<typename T, uint8_t base = 10> requires(std::integral<T>)
    [[nodiscard]] static constexpr T defineConversionInNumber(
        const std::string& number_in_form_of_string)
    {
        if constexpr (base != 16)
            errorsChunk<ErrorsChunks::CAN_NOT_CONVERTING_STRING_IN_INTEGER>(
                !checkStringInInt(number_in_form_of_string));

        using type = std::remove_cvref_t<T>;
        if constexpr (std::is_signed_v<type>)
        {
            if constexpr (sizeof(type) <= 4)
            {
                return static_cast<type>(std::stoi(number_in_form_of_string, nullptr, base));
            }
            else
            {
                return static_cast<type>(std::stoll(number_in_form_of_string, nullptr, base));
            }
        }
        else
        {
            if constexpr (sizeof(type) <= 4)
            {
                return static_cast<type>(std::stoul(number_in_form_of_string, nullptr, base));
            }
            else
            {
                return static_cast<type>(std::stoull(number_in_form_of_string, nullptr, base));
            }
        }
    }

    template<uint8_t size, typename T, uint8_t base = 10> requires(std::integral<T>)
    [[nodiscard]] constexpr T convertStringInInt(const char* const& string)
    {
        const auto* str = reinterpret_cast<const uint8_t*>(string);
        std::stringstream ss;
        for (auto i = str; i != str + size; ++i)
        {
            if (static_cast<T>(*i) && base == 16)
                ss << std::hex << static_cast<T>(*i);
            else
                ss << static_cast<T>(*i);
        }
        return defineConversionInNumber<T, base>(ss.str());
    }

    [[nodiscard]] static constexpr const char* subString(const uint32_t begin,
        const uint32_t end, const char* const& data)
    {
        errorsChunk<ErrorsChunks::CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING>(
            begin > end || begin == end);
        const auto sub = new char[end - begin + 1];
        for (uint32_t i = begin; i < end; i++)
            sub[i - begin] = data[i];
        sub[end - begin] = '\0';
        return sub;
    }

    template<typename T = decltype(_length),
             typename Enumclass = ErrorsChunks,
             Enumclass error = ErrorsChunks::CAN_NOT_REDEFINE_LENGTH_BLOCK,
             uint8_t n = 1>
    constexpr T covertFromArrayInInteger(const char* const& number_in_form_of_string,
        const bool val)
    {
        checkError<Enumclass, error, n>(std::forward<bool>(static_cast<bool>(val)));

        using inversionType = decltype(general_function::testInversion<T>());

        const inversionType u = convertStringInInt<DEFAULT_SIZE_BLOCK,
            inversionType>(number_in_form_of_string);

        errorsChunk<ErrorsChunks::THE_LENGTH_MUST_NOT_BE_LESS_THAN_ZERO>(u < 0);

        return static_cast<T>(u);
    }

    template<ErrorsChunks error>
    static constexpr void errorsChunk(bool&& val)
    {
        checkError<ErrorsChunks, error, 1>(std::forward<bool>(val));
    }

    constexpr void checkErrorsModeNotRead() const
    {
        using enum ErrorsChunks;
        errorsChunk<CAN_NOT_USING_FUNCTIONS_WRITES>(_mode == ModeChunk::WRITE);
        errorsChunk<CAN_NOT_USING_FUNCTIONS_NONE>(_mode == ModeChunk::NONE);
    }
    constexpr void checkErrorsModeNotWrite() const
    {
        using enum ErrorsChunks;
        errorsChunk<CAN_NOT_USING_FUNCTIONS_READS>(_mode == ModeChunk::READ);
        errorsChunk<CAN_NOT_USING_FUNCTIONS_NONE>(_mode == ModeChunk::NONE);
    }

public:
    constexpr explicit Chunk(const uint32_t begin, const uint32_t end,
                             const char* const& data, const ModeChunk& mode)
        : _name{}, _begin(begin), _end(end), _size(end - begin), _crc(0)
        , _length(0), _mode(mode), _isCalcLength(false), _isCalcTypeChunk(false)
        , _isCalcCrc(false), _data(nullptr), _isCanMultiChucks(false)
    {
        using enum ErrorsChunks;
        errorsChunk<CAN_NOT_WAS_CHUNK_SHORT_THAN_12_BYTES>(
            _size < DEFAULT_SIZE_BLOCK * 2);

        _length = covertFromArrayInInteger(
                subString(0, DEFAULT_SIZE_BLOCK, data), _isCalcLength);

        _isCalcLength = true;

        strcpy(_name, subString(DEFAULT_SIZE_BLOCK,
            DEFAULT_SIZE_BLOCK * 2, data));

        _data = subString(DEFAULT_SIZE_BLOCK * 2,
            _size - DEFAULT_SIZE_BLOCK, data);
        _crc = convertFromFromArrayInCrc(subString(
            _size - DEFAULT_SIZE_BLOCK, _size, data));

        errorsChunk<CHUNK_DAMAGED>(_crc == calcCrc(
            subString(4, 17, data), 17));
    }
    constexpr Chunk(const Chunk& other)
         : _name{}
         , _begin(other._begin)
         , _end(other._end)
         , _size(other._size)
         , _crc(other._crc)
         , _length(other._length)
         , _mode(other._mode)
         , _isCalcLength(other._isCalcLength)
         , _isCalcTypeChunk(other._isCalcTypeChunk)
         , _isCalcCrc(other._isCalcCrc)
         , _data(other._data)
         , _isCanMultiChucks(other._isCanMultiChucks)
    {
        strcpy(_name, other._name);
    }
    constexpr Chunk(Chunk&& other) noexcept
         : _name{}
         , _begin(other._begin)
         , _end(other._end)
         , _size(other._size)
         , _crc(other._crc)
         , _length(other._length)
         , _mode(other._mode)
         , _isCalcLength(other._isCalcLength)
         , _isCalcTypeChunk(other._isCalcTypeChunk)
         , _isCalcCrc(other._isCalcCrc)
         , _data(other._data)
         , _isCanMultiChucks(other._isCanMultiChucks)
    {
        strcpy(_name, other._name);
        strcpy(other._name, "");

        other._begin = 0;
        other._end = 0;
        other._size = 0;
        other._crc = 0;
        other._length = 0;
        other._mode = ModeChunk::NONE;
        other._isCanMultiChucks = false;
        other._isCalcLength = false;
        other._isCalcTypeChunk = false;
        other._isCalcCrc = false;
        other._data = nullptr;
    }
    constexpr virtual ~Chunk() = default;

    constexpr Chunk& operator=(const Chunk& other)
    {
         if (this == &other)
             return *this;

         _begin            = other._begin;
         _end              = other._end;
         _size             = other._size;
         _crc              = other._crc;
         _length           = other._length;
         _mode             = other._mode;
         _isCanMultiChucks = other._isCanMultiChucks;
         _isCalcLength     = other._isCalcLength;
         _isCalcTypeChunk  = other._isCalcTypeChunk;
         _isCalcCrc        = other._isCalcCrc;
         _data             = other._data;

        strcpy(_name, other._name);

         return *this;
    }
    constexpr Chunk& operator=(Chunk&& other) noexcept
    {
        if (this == &other)
            return *this;

        strcpy(_name, other._name);
        _begin            = other._begin;
        _end              = other._end;
        _size             = other._size;
        _crc              = other._crc;
        _length           = other._length;
        _mode             = other._mode;
        _isCanMultiChucks = other._isCanMultiChucks;
        _isCalcLength     = other._isCalcLength;
        _isCalcTypeChunk  = other._isCalcTypeChunk;
        _isCalcCrc        = other._isCalcCrc;
        _data             = other._data;

        strcpy(other._name, "");
        other._begin = 0;
        other._end = 0;
        other._size = 0;
        other._crc = 0;
        other._length = 0;
        other._mode = ModeChunk::NONE;
        other._isCanMultiChucks = false;
        other._isCalcLength = false;
        other._isCalcTypeChunk = false;
        other._isCalcCrc = false;
        other._data = nullptr;
        return *this;
    }

    [[nodiscard]] constexpr const char* name() const
    {
        return _name;
    }
    [[nodiscard]] constexpr uint32_t begin() const
    {
        return _begin;
    }
    [[nodiscard]] constexpr uint32_t end() const
    {
        return _end;
    }
    [[nodiscard]] constexpr uint32_t size() const
    {
        return _size;
    }
    [[nodiscard]] constexpr uint32_t crc() const
    {
        return _crc;
    }
    [[nodiscard]] constexpr uint16_t length() const
    {
        return _length;
    }

    template<bool isToStringMode = true>
    [[nodiscard]] constexpr auto mode() const
    {
        if constexpr (isToStringMode)
        {
            using enum ModeChunk;
            switch (_mode)
            {
                case NONE: return "NONE";
                case READ: return "READ";
                case WRITE: return "WRITE";
                case MIXED: return "MIXED";
                default: return "ERROR";
            }
        }
        else
            return _mode;
    }

    [[nodiscard]] constexpr bool isCanMultiChucks() const
    {
        return _isCanMultiChucks;
    }
};

} // chunks

#endif //CHUNK_HPP
