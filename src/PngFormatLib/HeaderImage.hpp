//
// Created by ganch on 26.11.2024.
//

#ifndef HERDERIMAGE_HPP
#define HERDERIMAGE_HPP

#include <cstdint>
#include <memory>

#include "Chunk.hpp"

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

    bool _isConvertWidthImage;
    bool _isConvertHeightImage;

    constexpr explicit HeaderImage(const uint32_t begin, const uint32_t end,
                          const char* const& data, ModeChunk&& mode)
        : Chunk(begin, end, data, std::forward<ModeChunk>(mode)), _widthImage(0)
        , _heightImage(0), _depth(0), _colorType(0), _compression(0), _filter(0)
        , _interface(0), _isConvertWidthImage(false), _isConvertHeightImage(false)
    {
        _widthImage = covertFromArrayInInteger<decltype(_widthImage)>(
            subString(0, DEFAULT_SIZE_BLOCK, _data), _isConvertWidthImage);
        _isConvertWidthImage = true;

        _heightImage = covertFromArrayInInteger<decltype(_heightImage)>(
            subString(DEFAULT_SIZE_BLOCK, DEFAULT_SIZE_BLOCK * 2, _data),
            _isConvertHeightImage);
        _isConvertHeightImage = true;

        _depth = static_cast<uint8_t>(_data[DEFAULT_SIZE_BLOCK * 2]);
        _colorType = static_cast<uint8_t>(_data[DEFAULT_SIZE_BLOCK * 2 + 1]);
        _compression = static_cast<uint8_t>(_data[DEFAULT_SIZE_BLOCK * 2 + 2]);
        _filter = static_cast<uint8_t>(_data[DEFAULT_SIZE_BLOCK * 2 + 3]);
        _interface = static_cast<uint8_t>(_data[DEFAULT_SIZE_BLOCK * 3]);
    }

    template<ErrorChunkHeaderImage error>
    static constexpr void checkErrorsHeaderImg(bool&& val)
    {
        checkError<ErrorChunkHeaderImage, error, 2>(std::forward<bool>(val));
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
      , _interface(other._interface)
      , _isConvertWidthImage(other._isConvertWidthImage)
      , _isConvertHeightImage(other._isConvertHeightImage) {}

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
        _isConvertWidthImage = other._isConvertWidthImage;
        _isConvertHeightImage = other._isConvertHeightImage;
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
        _isConvertWidthImage = other._isConvertWidthImage;
        _isConvertHeightImage = other._isConvertHeightImage;
        return *this;
    }

    static HeaderImage* create(const uint32_t begin,
        const uint32_t end, const char* const& data, ModeChunk&& mode)
    {
        checkErrorsHeaderImg<ErrorChunkHeaderImage::IT_SHOULD_NOT_BE_INITIALIZED_AGAIN>(
            _count != nullptr);
        if (_count == nullptr)
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
