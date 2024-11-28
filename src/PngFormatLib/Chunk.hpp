//
// Created by ganch on 25.11.2024.
//

#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <sstream>

#include "ErrorInfo.hpp"

namespace chunks {

static constexpr uint8_t DEFAULT_SIZE_BLOCK = 4;



class func_check_string_in_int_
{
public:
    template<typename T>
    constexpr bool operator()(const T& str) const
    {
        auto num = 0;
        for (auto i : str)
            if (isdigit(i))
                ++num;
        return num == str.size();
    }
};

inline constexpr func_check_string_in_int_ checkStringInInt{};



enum class ModeChunk : uint8_t
{
    NONE = 0, // default mode
    READ,
    WRITE,
    MIXED, // maybe I will
};

class Chunk
{
    char _name[DEFAULT_SIZE_BLOCK];
    uint32_t _begin;
    uint32_t _end;
    uint32_t _size;
    uint32_t _crc; // algorithm CRC-32
    uint16_t _length;

    ModeChunk _mode;
    TypesErrorsChunks _typeError;

    bool _isCalcLength;
    bool _isCalcTypeChunk;
    bool _isCalcCrc;

    constexpr uint32_t calcLength(const char* const& length)
    {
        errorsChunk(ErrorsChunks::CAN_NOT_REDEFINE_LENGTH_BLOCK,
                    std::forward<bool>(_isCalcLength));
         _isCalcLength = true;
        const int16_t u = convertStringInInt<DEFAULT_SIZE_BLOCK, int16_t>(length);
        errorsChunk(ErrorsChunks::THE_LENGTH_MUST_NOT_BE_LESS_THAN_ZERO, u < 0);
        return static_cast<decltype(_length)>(u);
    }
    constexpr uint32_t calcCrc(const char* const& crc)
    {
        errorsChunk(ErrorsChunks::CAN_NOT_REDEFINE_CRC_BLOCK,
            std::forward<bool>(_isCalcCrc));
        _isCalcCrc = true;
        return convertStringInInt<DEFAULT_SIZE_BLOCK, decltype(_crc)>(crc);
    }

protected:
    const char* _data;
    bool _isCanMultiChucks;

    template<typename T> requires(std::integral<T>)
    [[nodiscard]] constexpr T test_func(
        const std::string& number_in_form_of_string) const
    {
        errorsChunk(ErrorsChunks::CAN_NOT_CONVERTING_STRING_IN_INTEGER,
            !checkStringInInt(number_in_form_of_string));

        using type = std::remove_cvref_t<T>;
        if constexpr (std::is_signed_v<type>)
        {
            if constexpr (sizeof(type) <= 4)
                return static_cast<type>(std::stoi(number_in_form_of_string));
            else
                return static_cast<type>(std::stoll(number_in_form_of_string));
        }
        else
        {
            if constexpr (sizeof(type) <= 4)
                return static_cast<type>(std::stoul(number_in_form_of_string));
            else
                return static_cast<type>(std::stoull(number_in_form_of_string));
        }
    }

    template<uint8_t size, typename T> requires(std::integral<T>)
    [[nodiscard]] constexpr T convertStringInInt(const char* const& string)
    {
        std::stringstream ss;
        for (auto i = string; i != string + size; ++i)
            if (static_cast<T>(*i))
                ss << static_cast<T>(static_cast<uint8_t>(*i));
        return test_func<T>(ss.str());
    }

    [[nodiscard]] constexpr const char* subString(const uint32_t begin,
        const uint32_t end, const char* const& data) const
    {
        errorsChunk(ErrorsChunks::CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING,
            begin > end || begin == end);
        const auto sub = new char[end - begin + 1];
        for (uint32_t i = begin; i < end; i++)
            sub[i - begin] = data[i];
        sub[end - begin] = '\0';
        return sub;
    }

    constexpr void errorsChunk(ErrorsChunks&& errors_chunks, bool&& val) const
    {
        checkChunkUnderErrors(std::forward<ErrorsChunks>(errors_chunks),
            std::forward<bool>(val), _typeError);
    }

public:
    constexpr explicit Chunk(const uint32_t begin, const uint32_t end,
                             const char* data, const ModeChunk& mode,
                             const TypesErrorsChunks& type_error = TypesErrorsChunks::CHUNK)
        : _name{}, _begin(begin), _end(end)
        , _size(end - begin), _crc(0), _length(0), _mode(mode)
        , _typeError(type_error), _isCalcLength(false)
        , _isCalcTypeChunk(false), _isCalcCrc(false), _data(nullptr)
        , _isCanMultiChucks(false)
    {
        errorsChunk(ErrorsChunks::CAN_NOT_WAS_CHUNK_SHORT_THAN_12_BYTES,
            _size < DEFAULT_SIZE_BLOCK * 2);
        _length = calcLength(subString(0, DEFAULT_SIZE_BLOCK, data));
        strcpy(_name, subString(DEFAULT_SIZE_BLOCK,
            DEFAULT_SIZE_BLOCK * 2, data));
        _data = subString(DEFAULT_SIZE_BLOCK * 2, _length + 1, data);
        _crc = calcCrc(subString(_size - DEFAULT_SIZE_BLOCK, _size, data));
    }
    constexpr Chunk(const Chunk& other)
         : _name{}
         , _begin(other._begin)
         , _end(other._end)
         , _size(other._size)
         , _crc(other._crc)
         , _length(other._length)
         , _mode(other._mode)
         , _isCanMultiChucks(other._isCanMultiChucks)
         , _isCalcLength(other._isCalcLength)
         , _isCalcTypeChunk(other._isCalcTypeChunk)
         , _isCalcCrc(other._isCalcCrc)
         , _data(other._data)
         , _typeError(other._typeError)
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
         , _isCanMultiChucks(other._isCanMultiChucks)
         , _isCalcLength(other._isCalcLength)
         , _isCalcTypeChunk(other._isCalcTypeChunk)
         , _isCalcCrc(other._isCalcCrc)
         , _data(other._data)
         , _typeError(other._typeError)
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
        other._typeError = TypesErrorsChunks::NONE;
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
        _typeError         = other._typeError;

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
        _typeError         = other._typeError;

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
        other._typeError = TypesErrorsChunks::NONE;
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
    [[nodiscard]] constexpr auto testMode() const
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
