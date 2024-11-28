//
// Created by ganch on 26.11.2024.
//

#ifndef ERRORINFO_HPP
#define ERRORINFO_HPP

#include <array>
#include <cstdint>


namespace chunks
{

namespace critical_chunks
{


}


enum class ErrorsChunks : uint8_t
{
    NONE = 0, // if all is good

    CAN_NOT_REDEFINE_LENGTH_BLOCK,  // implemented in class Chunk
    CAN_NOT_REDEFINE_CRC_BLOCK,     // implemented in class Chunk
    CAN_NOT_REDEFINE_DATA_BLOCK,

    CAN_NOT_USING_FUNCTIONS_WRITES,
    CAN_NOT_USING_FUNCTIONS_READS,
    CAN_NOT_USING_FUNCTIONS_READS_OR_WRITES,

    CAN_NOT_CONVERTING_STRING_IN_INTEGER,   // implemented in class Chunk
    CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING,  // implemented in class Chunk
    CAN_NOT_WAS_CHUNK_SHORT_THAN_12_BYTES,  // implemented in class Chunk
    THE_LENGTH_MUST_NOT_BE_LESS_THAN_ZERO,  // implemented in class Chunk
};

// can be using modification constexpr
template<typename ENUMCLASS>
class ErrorInfo final : public std::exception
{
    ENUMCLASS _error;
    const char* _text{};

public:
    constexpr          ErrorInfo() noexcept = default;
    constexpr explicit ErrorInfo(const ENUMCLASS& err, const char* const&& text)
        : _error(err), _text(std::forward<const char* const>(text)) {}
    constexpr ~ErrorInfo() override = default;
    constexpr ErrorInfo(const ErrorInfo& other) = default;
    constexpr ErrorInfo(ErrorInfo&& other) noexcept
        : _error(other._error), _text(other._text)
    {
        other._error = ErrorsChunks::NONE;
        other._text  = nullptr;
    }

    [[nodiscard]] constexpr const char* what() const noexcept override
    {
        return _text;
    }

    constexpr ErrorInfo& operator=(const ErrorInfo& other)
    {
        if (this == &other)
            return *this;

        _error = other._error;
        _text  = other._text;

        return *this;
    }
    constexpr ErrorInfo& operator=(ErrorInfo&& other) noexcept
    {
        if (this == &other)
            return *this;

        _error = other._error;
        _text  = other._text;

        other._error = ErrorsChunks::NONE;
        other._text  = nullptr;

        return *this;
    }

    [[nodiscard]] constexpr ErrorsChunks error() const
    {
        return _error;
    }
    [[nodiscard]] constexpr const char* text() const
    {
        return _text;
    }
};

static constexpr uint8_t SIZE_ARRAY_ERRORS_CHUNKS = 10;
static inline const ErrorInfo<ErrorsChunks> ERRORS[SIZE_ARRAY_ERRORS_CHUNKS]
{
    // Errors -> redefine blocks
    ErrorInfo(ErrorsChunks::CAN_NOT_REDEFINE_LENGTH_BLOCK,
              "Can`t redefine length block!!!"),
    ErrorInfo(ErrorsChunks::CAN_NOT_REDEFINE_CRC_BLOCK,
              "Can`t redefine CRC block!!!"),
    ErrorInfo(ErrorsChunks::CAN_NOT_REDEFINE_DATA_BLOCK,
              "Can`t redefine data block!!!"),

    // Error -> mode chunk
    ErrorInfo(ErrorsChunks::CAN_NOT_USING_FUNCTIONS_READS,
              "Can`t using functions reads!!!"),
    ErrorInfo(ErrorsChunks::CAN_NOT_USING_FUNCTIONS_WRITES,
              "Can`t using functions writes!!!"),
    ErrorInfo(ErrorsChunks::CAN_NOT_USING_FUNCTIONS_READS_OR_WRITES,
              "Can`t using functions reads or writes!!!"),

    // Error -> internal function class Chunk
    ErrorInfo(ErrorsChunks::CAN_NOT_CONVERTING_STRING_IN_INTEGER,
              "Can`t converting string in integer!!!"),
    ErrorInfo(ErrorsChunks::CAN_NOT_WAS_CHUNK_SHORT_THAN_12_BYTES,
              "Can`t was chunk short than 12 bytes!!!"),
    ErrorInfo(ErrorsChunks::CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING,
              "Can`t was crash string on substring!!!"),
    ErrorInfo(ErrorsChunks::THE_LENGTH_MUST_NOT_BE_LESS_THAN_ZERO,
              "The length must not be less than zero!!!"),
};


namespace critical_chunks
{

// static constexpr std::array

}

template<auto number> requires(std::is_integral_v<decltype(number)>)
struct func_defined_type
{
    constexpr auto operator()() const noexcept
    {
        if constexpr (number < INT8_MAX && number > INT8_MIN)
            return static_cast<int8_t>(number);
        else if constexpr (number < UINT8_MAX && number > 0)
            return static_cast<uint8_t>(number);
        else if constexpr (number < INT16_MAX && number > INT16_MIN)
            return static_cast<int16_t>(number);
        else if constexpr (number < UINT16_MAX && number > 0)
            return static_cast<uint16_t>(number);
        else if constexpr (number < INT32_MAX && number > INT32_MIN)
            return static_cast<int32_t>(number);
        else if constexpr (number < UINT32_MAX && number > 0)
            return static_cast<uint32_t>(number);
        else if constexpr (number < INT64_MAX && number > INT64_MIN)
            return static_cast<int64_t>(number);
        else if constexpr (number < UINT64_MAX && number > 0)
            return static_cast<uint64_t>(number);
    };
};

template<auto number> requires(std::is_integral_v<decltype(number)>)
inline constexpr auto func_defined_type_t = func_defined_type<number>();


enum class TypesErrorsChunks : uint8_t
{
    NONE = 0,
    CHUNK,
    HEADER_IMAGE_CHUNK,
};

class func_check_errors_
{
    template<typename ENUMCLASS>
    struct Typ
    {
        const ErrorInfo<ENUMCLASS>* const& arr;
        const uint8_t size_arr;
    };

    template<typename ENUMCLASS>
    static constexpr Typ<ENUMCLASS> define_type_error(const TypesErrorsChunks& type)
    {
        using enum TypesErrorsChunks;
        switch (type)
        {
            case CHUNK: return Typ<ENUMCLASS>{ERRORS, SIZE_ARRAY_ERRORS_CHUNKS};
            case HEADER_IMAGE_CHUNK:
                default: return Typ<ENUMCLASS>{new ErrorInfo<ENUMCLASS>[1], 1};
        }
    };

public:
    template<typename ENUMCLASS> requires(std::is_enum_v<ENUMCLASS>)
    constexpr void operator()(const ENUMCLASS& error, const bool value,
        const TypesErrorsChunks& type) const
    {
        const Typ<ENUMCLASS>& t = define_type_error<ENUMCLASS>(type);
        for (auto i = t.arr; i != t.arr + t.size_arr; ++i)
            if (i->error() == error && value)
                throw *i; // rewrite
    }
};

static inline constexpr func_check_errors_ checkChunkUnderErrors{};

} // chunks

#endif //ERRORINFO_HPP
