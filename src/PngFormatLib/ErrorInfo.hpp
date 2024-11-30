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

enum class ErrorChunkHeaderImage : uint8_t
{
    NONE = 0,

    IT_SHOULD_NOT_BE_INITIALIZED_AGAIN,

};

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
template<typename Enumclass>
class ErrorInfo
{
    Enumclass _error;
    const char* _text{};

public:
    constexpr          ErrorInfo() noexcept = default;
    constexpr explicit ErrorInfo(Enumclass&& err, const char* const&& text)
        : _error(std::forward<Enumclass>(err))
        , _text(std::forward<const char* const>(text)) {}
    constexpr ~ErrorInfo() = default;
    constexpr ErrorInfo(const ErrorInfo& other) = default;
    constexpr ErrorInfo(ErrorInfo&& other) noexcept
        : _error(other._error), _text(other._text)
    {
        other._error = ErrorsChunks::NONE;
        other._text  = nullptr;
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
static constexpr ErrorInfo<ErrorsChunks> GENERAL_ERRORS_CHUNK[SIZE_ARRAY_ERRORS_CHUNKS]
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
              "Can't use functions to read or write!!!"),

    // Error -> internal function class Chunk
    ErrorInfo(ErrorsChunks::CAN_NOT_CONVERTING_STRING_IN_INTEGER,
              "Can't convert string to integer!!!"),
    ErrorInfo(ErrorsChunks::CAN_NOT_WAS_CHUNK_SHORT_THAN_12_BYTES,
              "Can't be chunk shorter than 12 bytes!!!"),
    ErrorInfo(ErrorsChunks::CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING,
              "Can't crash string on substring!!!"),
    ErrorInfo(ErrorsChunks::THE_LENGTH_MUST_NOT_BE_LESS_THAN_ZERO,
              "The length mustn't be less than zero!!!"),
};


namespace critical_chunks
{

static constexpr uint8_t SIZE_ARRAY_ERRORS_CHUNKS = 1;
static constexpr ErrorInfo<ErrorChunkHeaderImage>
    ERRORS_HEADER_IMAGE[SIZE_ARRAY_ERRORS_CHUNKS]
{
    ErrorInfo(ErrorChunkHeaderImage::IT_SHOULD_NOT_BE_INITIALIZED_AGAIN,
              "It should not be initialized again"),
};

}


enum class TypesErrorsChunks : uint8_t
{
    NONE = 0,
    CHUNK,
    HEADER_IMAGE_CHUNK,
};

class FuncCheckErrors
{
    template<typename Enumclass>
    struct Typ
    {
        const ErrorInfo<Enumclass>* const& arr;
        const uint8_t size_arr;
    };

    template<typename Enumclass>
    static constexpr Typ<Enumclass> defineTypeError(const TypesErrorsChunks& type)
    {
        using enum TypesErrorsChunks;
        switch (type)
        {
            case CHUNK: return Typ<Enumclass>{GENERAL_ERRORS_CHUNK, SIZE_ARRAY_ERRORS_CHUNKS};
            case HEADER_IMAGE_CHUNK:
                default: return Typ<Enumclass>{new ErrorInfo<Enumclass>[1], 1};
        }
    };

public:
    template<typename Enumclass> requires(std::is_enum_v<Enumclass>)
    constexpr void operator()(const Enumclass& error, const bool value,
        const TypesErrorsChunks& type) const
    {
        const Typ<Enumclass>& t = defineTypeError<Enumclass>(type);
        for (auto i = t.arr; i != t.arr + t.size_arr; ++i)
        {
            if (i->error() == error && value)
            {
                throw std::runtime_error(i->text()); // rewrite
            }
        }
    }
};


inline constexpr FuncCheckErrors checkChunkUnderErrors{};

} // chunks

#endif //ERRORINFO_HPP
