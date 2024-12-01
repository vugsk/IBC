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

    THE_WIDTH_IMAGE_MUST_NOT_BE_LESS_THAN_ZERO,
    THE_HEIGHT_IMAGE_MUST_NOT_BE_LESS_THAN_ZERO,

};

}


enum class ErrorsChunks : uint8_t
{
    NONE = 0, // if all is good

    CAN_NOT_REDEFINE_LENGTH_BLOCK,  // implemented in class Chunk
    CAN_NOT_REDEFINE_CRC_BLOCK,     // implemented in class Chunk
    CAN_NOT_REDEFINE_DATA_BLOCK,
    CHUNK_DAMAGED,

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
        other._error = Enumclass::NONE;
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

        other._error = Enumclass::NONE;
        other._text  = nullptr;

        return *this;
    }

    [[nodiscard]] constexpr Enumclass error() const
    {
        return _error;
    }
    [[nodiscard]] constexpr const char* const& text() const
    {
        return _text;
    }
};

static constexpr std::array GENERAL_ERRORS_CHUNK
{
    // Errors -> redefine blocks
    ErrorInfo(ErrorsChunks::CAN_NOT_REDEFINE_LENGTH_BLOCK,
              "Can`t redefine length block!!!"),
    ErrorInfo(ErrorsChunks::CAN_NOT_REDEFINE_CRC_BLOCK,
              "Can`t redefine CRC block!!!"),
    ErrorInfo(ErrorsChunks::CAN_NOT_REDEFINE_DATA_BLOCK,
              "Can`t redefine data block!!!"),
    ErrorInfo(ErrorsChunks::CHUNK_DAMAGED,
              "Chunk to damaged!!!"),

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

static constexpr std::array ERRORS_HEADER_IMAGE =
{
    ErrorInfo(ErrorChunkHeaderImage::IT_SHOULD_NOT_BE_INITIALIZED_AGAIN,
              "It shouldn't be initialized again"),
    ErrorInfo(ErrorChunkHeaderImage::THE_WIDTH_IMAGE_MUST_NOT_BE_LESS_THAN_ZERO,
            "The width image mustn't be less than zero!!!"),
    ErrorInfo(ErrorChunkHeaderImage::THE_HEIGHT_IMAGE_MUST_NOT_BE_LESS_THAN_ZERO,
            "The height image mustn't be less than zero!!!"),

};

}


template<typename Enumclass, Enumclass error, uint8_t n>
class check_errors_fn
{
    enum class TypesErrorsChunks : uint8_t
    {
        NONE = 0,
        CHUNK,
        HEADER_IMAGE_CHUNK,
    };

    static constexpr TypesErrorsChunks defineEnumTypesErrors(const uint8_t type)
    {
        switch (type)
        {
            case 1: return TypesErrorsChunks::CHUNK;
            case 2: return TypesErrorsChunks::HEADER_IMAGE_CHUNK;
            default: return TypesErrorsChunks::NONE;
        }
    }

    template<TypesErrorsChunks type>
    static constexpr auto defineArrayWithErrors()
    {
        if constexpr (type == TypesErrorsChunks::CHUNK)
            return GENERAL_ERRORS_CHUNK;
        else if constexpr (type == TypesErrorsChunks::HEADER_IMAGE_CHUNK)
            return critical_chunks::ERRORS_HEADER_IMAGE;
        else
            return std::array<TypesErrorsChunks, 1>();
    }

    static constexpr auto array_e = defineArrayWithErrors<defineEnumTypesErrors(n)>();

    // loop -> for
    template<auto start = 0, auto end = array_e.size(), auto step = 1>
    static constexpr void iterationArrayWithError()
    {
        if constexpr (step > 0 && start < end)
        {
            if constexpr (array_e[start].error() == error)
                throw std::runtime_error(array_e[start].text());
            iterationArrayWithError<start + step, end>();
        }
    }

public:
    constexpr check_errors_fn() = default;
    constexpr ~check_errors_fn() = default;

    constexpr void operator()(bool&& val) const
    {
        if (val)
            iterationArrayWithError();
    }
};

template<typename Enumclass, Enumclass error, uint8_t n>
static inline constexpr check_errors_fn<Enumclass, error, n> checkError{};

} // chunks

#endif //ERRORINFO_HPP
