//
// Created by ganch on 26.11.2024.
//

#ifndef ERRORINFO_HPP
#define ERRORINFO_HPP

#include <array>
#include <cstdint>
#include <stdexcept>


namespace chunks {

enum class Errors : uint8_t
{
    NONE = 0, // if all is good

    CAN_NOT_REDEFINE_LENGHT_BLOCK, // implemented in class Chunk
    CAN_NOT_REDEFINE_CRC_BLOCK,     // implemented in class Chunk
    CAN_NOT_REDEFINE_DATA_BLOCK,

    CAN_NOT_USING_FUNCTIONS_WRITES,
    CAN_NOT_USING_FUNCTIONS_READS,
    CAN_NOT_USING_FUNCTIONS_READS_OR_WRITES,

    CAN_NOT_CONVERTING_STRING_IN_INTEGER,
    CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING, // implemented
    CAN_NOT_WAS_CHUNK_SHORT_THAN_12_BYTES,
    THE_LENGTH_MUST_NOT_BE_LESS_THAN_ZERO,
};

// can be using modification constexpr
class ErrorInfo
{
    Errors _error;
    const char* _text;

public:
    constexpr          ErrorInfo() = default;
    constexpr explicit ErrorInfo(Errors err, const char* const&& text)
        : _error(err), _text(std::forward<const char* const>(text)) {}
    constexpr ~ErrorInfo() = default;
    constexpr ErrorInfo(const ErrorInfo& other) = default;
    constexpr ErrorInfo(ErrorInfo&& other) noexcept
        : _error(other._error), _text(other._text)
    {
        other._error = Errors::NONE;
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

        other._error = Errors::NONE;
        other._text  = nullptr;

        return *this;
    }

    [[nodiscard]] constexpr Errors error() const
    {
        return _error;
    }
    [[nodiscard]] constexpr const char* text() const
    {
        return _text;
    }
};

static constexpr std::array ERRORS
{
    // Errors -> redefine blocks
    ErrorInfo(Errors::CAN_NOT_REDEFINE_LENGHT_BLOCK,
              "Can`t redefine lenght block!!!"),
    ErrorInfo(Errors::CAN_NOT_REDEFINE_CRC_BLOCK,
              "Can`t redefine CRC block!!!"),
    ErrorInfo(Errors::CAN_NOT_REDEFINE_DATA_BLOCK,
              "Can`t redefine data block!!!"),

    // Error -> mode chunk
    ErrorInfo(Errors::CAN_NOT_USING_FUNCTIONS_READS,
              "Can`t using functions reads!!!"),
    ErrorInfo(Errors::CAN_NOT_USING_FUNCTIONS_WRITES,
              "Can`t using functions writes!!!"),
    ErrorInfo(Errors::CAN_NOT_USING_FUNCTIONS_READS_OR_WRITES,
              "Can`t using functions reads or writes!!!"),

    // Error -> internal function class Chunk
    ErrorInfo(Errors::CAN_NOT_CONVERTING_STRING_IN_INTEGER,
              "Can`t converting string in integer!!!"),
    ErrorInfo(Errors::CAN_NOT_WAS_CHUNK_SHORT_THAN_12_BYTES,
              "Can`t was chunk short than 12 bytes!!!"),
    ErrorInfo(Errors::CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING,
              "Can`t was crash string on substring!!!"),
    ErrorInfo(Errors::THE_LENGTH_MUST_NOT_BE_LESS_THAN_ZERO,
              "The length must not be less than zero!!!"),
};

static constexpr void ErrorsChunk(const Errors error, const bool value)
{
    for (auto i : ERRORS)
        if (i.error() == error && value)
            throw std::runtime_error(i.text());
}

} // chunks

#endif //ERRORINFO_HPP
