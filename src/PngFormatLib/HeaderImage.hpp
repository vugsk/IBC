//
// Created by ganch on 26.11.2024.
//

#ifndef HERDERIMAGE_HPP
#define HERDERIMAGE_HPP

#include <cstdint>

#include "Chunk.hpp"
#include "ErrorObject.hpp"

namespace chunks::critical_chunks
{

class HeaderImage final : public Chunk
{
    static HeaderImage* _count;

    uint16_t _widthImage;
    uint16_t _heightImage;
    uint8_t _depth;
    uint8_t _colorType;
    uint8_t _compression;
    uint8_t _filter;
    uint8_t _interface;

    template<typename T, ErrorChunkHeaderImage error, typename T2>
    constexpr T covertFromArrayInInteger(
        const char* const& number_in_form_of_string, T2&& valueConvertingInInt)
    {
        return Chunk::covertFromArrayInInteger<T, ErrorChunkHeaderImage, error, 2>(
            number_in_form_of_string, valueConvertingInInt);
    }

    constexpr explicit HeaderImage(const uint32_t begin, const uint32_t end,
                          const char* const& data, ModeChunk&& mode)
        : Chunk(begin, end, data, std::forward<ModeChunk>(mode)), _widthImage(0)
        , _heightImage(0), _depth(0), _colorType(0), _compression(0), _filter(0)
        , _interface(0)
    {
        using enum ErrorChunkHeaderImage;
        using width_img = decltype(_widthImage);
        using height_img = decltype(_heightImage);

        _widthImage = covertFromArrayInInteger<width_img,
                CAN_NOT_REDEFINE_WIDTH_IMAGE>(
            subString(0, SIZE_ONE_BLOCK, _data), _widthImage);

        _heightImage = covertFromArrayInInteger<height_img,
                CAN_NOT_REDEFINE_HEIGHT_IMAGE>(
            subString(SIZE_ONE_BLOCK, SIZE_SECOND_BLOCKS, _data),
            _heightImage);

        _depth = static_cast<uint8_t>(_data[SIZE_SECOND_BLOCKS]);
        _colorType = static_cast<uint8_t>(_data[SIZE_SECOND_BLOCKS + 1]);
        _compression = static_cast<uint8_t>(_data[SIZE_SECOND_BLOCKS + 2]);
        _filter = static_cast<uint8_t>(_data[SIZE_SECOND_BLOCKS + 3]);
        _interface = static_cast<uint8_t>(_data[SIZE_SECOND_BLOCKS + 3]);
    }

    template<ErrorChunkHeaderImage Error>
    static constexpr void checkErrorsHeaderImg(bool&& val)
    {
        if (std::forward<bool>(val))
            error_information::checkError<ErrorChunkHeaderImage, Error,
                                          Test_chunk<2>, true, true>();
    }

public:
    constexpr ~HeaderImage() override = default;

    constexpr HeaderImage(const HeaderImage& other) = default;
    constexpr HeaderImage(HeaderImage&& other) noexcept
        : Chunk(std::move(other))
      , _widthImage(other._widthImage)
      , _heightImage(other._heightImage)
      , _depth(other._depth)
      , _colorType(other._colorType)
      , _compression(other._compression)
      , _filter(other._filter)
      , _interface(other._interface) {}

    constexpr HeaderImage& operator=(const HeaderImage& other)
    {
        if (this == &other)
            return *this;
        Chunk::operator =(other);
        _widthImage  = other._widthImage;
        _heightImage = other._heightImage;
        _depth       = other._depth;
        _colorType   = other._colorType;
        _compression = other._compression;
        _filter      = other._filter;
        _interface   = other._interface;
        return *this;
    }
    constexpr HeaderImage& operator=(HeaderImage&& other) noexcept
    {
        if (this == &other)
            return *this;
        Chunk::operator =(std::move(other));
        _widthImage  = other._widthImage;
        _heightImage = other._heightImage;
        _depth       = other._depth;
        _colorType   = other._colorType;
        _compression = other._compression;
        _filter      = other._filter;
        _interface   = other._interface;
        return *this;
    }

    __forceinline static HeaderImage* create(const uint32_t begin, const uint32_t end,
        const char* const& data, ModeChunk&& mode)
    {
        using enum ErrorChunkHeaderImage;
        checkErrorsHeaderImg<IT_SHOULD_NOT_BE_INITIALIZED_AGAIN>(_count);
        if (!_count)
            _count = new HeaderImage(begin, end, data, std::forward<ModeChunk>(mode));
        return _count;
    }

    [[nodiscard]] constexpr uint16_t widthImage() const
    {
        checkErrorsModeNotRead();
        return _widthImage;
    }
    [[nodiscard]] constexpr uint16_t heightImage() const
    {
        checkErrorsModeNotRead();
        return _heightImage;
    }
    [[nodiscard]] constexpr uint8_t depth() const
    {
        checkErrorsModeNotRead();
        return _depth;
    }
    [[nodiscard]] constexpr uint8_t colorType() const
    {
        checkErrorsModeNotRead();
        return _colorType;
    }
    [[nodiscard]] constexpr uint8_t compression() const
    {
        checkErrorsModeNotRead();
        return _compression;
    }
    [[nodiscard]] constexpr uint8_t filter() const
    {
        checkErrorsModeNotRead();
        return _filter;
    }
    [[nodiscard]] constexpr uint8_t interface() const
    {
        checkErrorsModeNotRead();
        return _interface;
    }
};


}

#endif //HERDERIMAGE_HPP
