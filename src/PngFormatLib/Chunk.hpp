//
// Created by ganch on 25.11.2024.
//

#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <cstdint>
#include <cstring>
#include <sstream>

#include "ErrorInformationChunks.hpp"
#include "GeneralFunction.hpp"

namespace chunks
{

class func_check_string_in_int_
{
public:
    template<typename T>
    constexpr bool operator()(T&& str) const
        requires(std::is_same_v<std::remove_cvref_t<T>, std::string> ||
                 std::is_same_v<T, const char*>)
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
protected:
    static constexpr uint8_t SIZE_ONE_BLOCK = 4;
    static constexpr uint8_t SIZE_SECOND_BLOCKS = SIZE_ONE_BLOCK * 2;

private:
    char _name[SIZE_ONE_BLOCK];
    uint32_t _begin;
    uint32_t _end;
    uint32_t _size;
    uint32_t _crc;
    uint16_t _length;

    ModeChunk _mode;

    constexpr auto convertFromFromArrayInCrc(const char* const& crc) -> decltype(_crc)
    {
        redefineBlockWithChunk<ErrorsChunks::CAN_NOT_REDEFINE_CRC_BLOCK>(_crc);
        return convertStringInInt<SIZE_ONE_BLOCK, decltype(_crc), 16>(crc);
    }

    __forceinline static constexpr uint32_t calcCrc(const char*&& buf, uint64_t&& len)
    {
        uint64_t crc_table[256];
        uint64_t crc;
        for (uint16_t i = 0; i < 256; i++)
        {
            crc = i;
            for (uint8_t j = 0; j < 8; j++)
                crc = (crc >> 1) ^ (crc & 1 ? 0xEDB88320UL : 0);
            crc_table[i] = crc;
        };

        crc = 0xFFFFFFFFUL;
        while (len--)
            crc = (crc >> 8) ^ crc_table[(crc ^ *buf++) & 0xFF];
        return crc ^ 0xFFFFFFFFUL;
    }

    template<ModeChunk Mode>
    constexpr void ste() const
    {
        using enum ErrorsChunks;
        if (_mode == Mode)
            errorsChunk<CAN_NOT_USING_FUNCTIONS_WRITES>(true);
        else
            errorsChunk<CAN_NOT_USING_FUNCTIONS_NONE>(_mode == ModeChunk::NONE);
    }

protected:
    bool _isCanMultiChucks;
    const char* _data;

    template<typename Enumclass, uint8_t typeErrors, Enumclass error,
             bool valueForWhichYouNeedToCallError = true, typename T>
    static constexpr void redefineBlockWithChunk(T&& value)
        requires(!std::is_pointer_v<T>, !std::is_class_v<T>)
    {
        if (value && valueForWhichYouNeedToCallError)
            error_information::checkError<Enumclass, error, Test_chunk<typeErrors>,
                            true, valueForWhichYouNeedToCallError>();
    }

    template<ErrorsChunks error, bool valueForWhichYouNeedToCallError = true,
             typename T>
    __forceinline static constexpr void redefineBlockWithChunk(T&& value)
        requires(!std::is_pointer_v<T>, !std::is_class_v<T>)
    {
        redefineBlockWithChunk<ErrorsChunks, 1, error,
                                valueForWhichYouNeedToCallError>(value);
    }

    template<typename T, uint8_t Base = 10> requires(std::integral<T>)
    [[nodiscard]] static constexpr T defineConversionInNumber(
        const std::string& number_in_form_of_string)
    {
        if constexpr (Base != 16)
            errorsChunk<ErrorsChunks::CAN_NOT_CONVERTING_STRING_IN_INTEGER>(
                !checkStringInInt(number_in_form_of_string));

        using type = std::remove_cvref_t<T>;
        if constexpr (std::is_signed_v<type>)
        {
            if constexpr (sizeof(type) <= 4)
                return static_cast<type>(std::stoi(
                    number_in_form_of_string, nullptr, Base));
            else
                return static_cast<type>(std::stoll(
                    number_in_form_of_string, nullptr, Base));
        }
        else if constexpr (sizeof(type) <= 4)
            return static_cast<type>(std::stoul(
                number_in_form_of_string, nullptr, Base));
        else
            return static_cast<type>(std::stoull(
                number_in_form_of_string, nullptr, Base));
    }

    template<uint8_t Size, typename T, uint8_t Base = 10>
    [[nodiscard]] static constexpr T convertStringInInt(const char* const& string)
        requires(std::integral<T>)
    {
        const auto* str = reinterpret_cast<const uint8_t*>(string);
        std::stringstream ss;
        for (auto i = str; i != str + Size; ++i)
        {
            if (static_cast<T>(*i) && Base == 16)
                ss << std::hex << static_cast<T>(*i);
            else
                ss << static_cast<T>(*i);
        }
        return defineConversionInNumber<T, Base>(ss.str());
    }

    [[nodiscard]] __forceinline static constexpr const char* subString(
        const uint32_t begin, const uint32_t end, const char* const& data)
    {
        return general_function::SubString<ErrorsChunks,
            ErrorsChunks::CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING, Test_chunk<1>>(
                begin, end, data);
    }

    template<typename T = decltype(_length),
             typename Enumclass = ErrorsChunks,
             Enumclass error = ErrorsChunks::CAN_NOT_REDEFINE_LENGTH_BLOCK,
             uint8_t n = 1, typename T2 = T>
    [[nodiscard]] static constexpr T covertFromArrayInInteger(
        const char* const& number_in_form_of_string, T2&& valueConvertingInInt)
    {
        redefineBlockWithChunk<Enumclass, n, error>(valueConvertingInInt);
        using inversionType = decltype(general_function::testInversion<T>());

        const inversionType u = convertStringInInt<SIZE_ONE_BLOCK,
            inversionType>(number_in_form_of_string);

        errorsChunk<ErrorsChunks::THE_LENGTH_MUST_NOT_BE_LESS_THAN_ZERO>(u < 0);

        return static_cast<T>(u);
    }

    template<ErrorsChunks Error>
    static constexpr void errorsChunk(bool&& val)
    {
        error_information::checkError<ErrorsChunks, Error, Test_chunk<1>>(
            std::forward<bool>(val));
    }

    __forceinline constexpr void checkErrorsModeNotRead() const
    {
        ste<ModeChunk::WRITE>();
    }
    __forceinline constexpr void checkErrorsModeNotWrite() const
    {
        ste<ModeChunk::READ>();
    }

public:
    constexpr explicit Chunk(const uint32_t begin, const uint32_t end,
                             const char* const& data, const ModeChunk& mode)
        : _name{}, _begin(begin), _end(end), _size(end - begin), _crc(0)
        , _length(0), _mode(mode), _isCanMultiChucks(false), _data(nullptr)
    {
        using enum ErrorsChunks;
        errorsChunk<CAN_NOT_WAS_CHUNK_SHORT_THAN_12_BYTES>(
            _size < SIZE_ONE_BLOCK * 2);

        _length = covertFromArrayInInteger(
                subString(0, SIZE_ONE_BLOCK, data), _length);

        strcpy(_name, subString(SIZE_ONE_BLOCK,
            SIZE_ONE_BLOCK * 2, data));

        _data = subString(SIZE_ONE_BLOCK * 2,
            _size - SIZE_ONE_BLOCK, data);
        _crc = convertFromFromArrayInCrc(subString(
            _size - SIZE_ONE_BLOCK, _size, data));

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
        _data             = other._data;

        strcpy(other._name, "");
        other._begin = 0;
        other._end = 0;
        other._size = 0;
        other._crc = 0;
        other._length = 0;
        other._mode = ModeChunk::NONE;
        other._isCanMultiChucks = false;
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
