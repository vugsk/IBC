//
// Created by ganch on 05.12.2024.
//

#ifndef SIGNATURE_HPP
#define SIGNATURE_HPP

#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "ErrorObject.hpp"
#include "GeneralFunction.hpp"


namespace signature
{

enum class ErrorSignature : uint8_t
{
    NONE = 0,

    FILE_IS_NOT_IN_PNG_FORMAT,
    IT_SHOULD_NOT_BE_INITIALIZED_AGAIN_SIGNATURE,
    CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING_IN_SIGNATURE,

};

static constexpr std::array ERRORS_SIGNATURE =
{
    error_information::ErrorInformationChunks(
        ErrorSignature::FILE_IS_NOT_IN_PNG_FORMAT,
        "File is not in png format!!!"),
    error_information::ErrorInformationChunks(
        ErrorSignature::IT_SHOULD_NOT_BE_INITIALIZED_AGAIN_SIGNATURE,
        "It shouldn't be initialized again signature!!!"),
    error_information::ErrorInformationChunks(
        ErrorSignature::CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING_IN_SIGNATURE,
        "Can't was crash string on substring in signature!!!"),
};

class Signature
{
    static constexpr uint8_t RIGHT_VALUE_NON_ASCII = 137;
    static constexpr auto RIGHT_VALUE_TYPE = "PNG";
    static constexpr auto TET = []{ return ERRORS_SIGNATURE; };

    static Signature* _signature;

    uint32_t _beginPosition;
    uint32_t _endPosition;
    uint32_t _size;

    char _type[4]{};
    char _dosStyle[2]{};
    uint8_t _nonAscii;
    uint8_t _stopOutputFileDos;
    uint8_t _unixStyle;

    constexpr Signature(const uint32_t begin, const uint32_t end,
                        const char* const& data)
        : _beginPosition(begin), _endPosition(end), _size(end - begin)
        , _nonAscii(0), _stopOutputFileDos(0), _unixStyle(0)
    {
        using enum ErrorSignature;
        strcpy(_type, subString(1, 4, data));
        checkErrorsSignature<FILE_IS_NOT_IN_PNG_FORMAT>(
            strcmp(_type, RIGHT_VALUE_TYPE) != 0);

        strcpy(_dosStyle, subString(4, 6, data));
        _stopOutputFileDos = data[6];
        _unixStyle = data[7];

        _nonAscii = data[0];
        checkErrorsSignature<FILE_IS_NOT_IN_PNG_FORMAT>(
            _nonAscii != RIGHT_VALUE_NON_ASCII);
    }

    [[nodiscard]] __forceinline static constexpr const char* subString(
        const uint32_t begin, const uint32_t end, const char* const& data)
    {
        return general_function::SubString<ErrorSignature,
            ErrorSignature::CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING_IN_SIGNATURE,
            decltype(TET)>(begin, end, data);
    }

    template<ErrorSignature Error>
    static constexpr void checkErrorsSignature(bool&& val)
    {
        if (std::forward<bool>(val))
            error_information::checkError<ErrorSignature, Error, decltype(TET),
                    true, true>();
    }

public:
    constexpr ~Signature() = default;

    Signature(const Signature& other)
        : _beginPosition(other._beginPosition)
      , _endPosition(other._endPosition)
      , _size(other._size)
      , _nonAscii(other._nonAscii)
      , _stopOutputFileDos(other._stopOutputFileDos)
      , _unixStyle(other._unixStyle) {}
    Signature(Signature&& other) noexcept
        : _beginPosition(other._beginPosition)
      , _endPosition(other._endPosition)
      , _size(other._size)
      , _nonAscii(other._nonAscii)
      , _stopOutputFileDos(other._stopOutputFileDos)
      , _unixStyle(other._unixStyle) {}

    Signature& operator=(const Signature& other)
    {
        if (this == &other)
            return *this;
        _beginPosition     = other._beginPosition;
        _endPosition       = other._endPosition;
        _size              = other._size;
        _nonAscii          = other._nonAscii;
        _stopOutputFileDos = other._stopOutputFileDos;
        _unixStyle         = other._unixStyle;
        return *this;
    }
    Signature& operator=(Signature&& other) noexcept
    {
        if (this == &other)
            return *this;
        _beginPosition     = other._beginPosition;
        _endPosition       = other._endPosition;
        _size              = other._size;
        _nonAscii          = other._nonAscii;
        _stopOutputFileDos = other._stopOutputFileDos;
        _unixStyle         = other._unixStyle;
        return *this;
    }

    [[nodiscard]] __forceinline static Signature* create(const uint32_t begin,
        const uint32_t end, const char* const& data)
    {
        using enum ErrorSignature;
        checkErrorsSignature<IT_SHOULD_NOT_BE_INITIALIZED_AGAIN_SIGNATURE>(_signature);
        if (!_signature)
            _signature = new Signature(begin, end, data);
        return _signature;
    }

    [[nodiscard]] constexpr uint32_t getBeginPosition() const
    {
        return _beginPosition;
    }
    [[nodiscard]] constexpr uint32_t getEndPosition() const
    {
        return _endPosition;
    }
    [[nodiscard]] constexpr uint32_t getSize() const
    {
        return _size;
    }
    [[nodiscard]] constexpr uint8_t getNonAscii() const
    {
        return _nonAscii;
    }
    [[nodiscard]] constexpr uint8_t getStopOutputFileDos() const
    {
        return _stopOutputFileDos;
    }
    [[nodiscard]] constexpr uint8_t getUnixStyle() const
    {
        return _unixStyle;
    }
    [[nodiscard]] constexpr const char* getDosStyle() const
    {
        return _dosStyle;
    }
    [[nodiscard]] constexpr const char* getType() const
    {
        return _type;
    }
};

}

#endif //SIGNATURE_HPP
