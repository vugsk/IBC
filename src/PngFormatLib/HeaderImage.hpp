//
// Created by ganch on 26.11.2024.
//

#ifndef HERDERIMAGE_HPP
#define HERDERIMAGE_HPP

#include <cstdint>

#include "Chunk.hpp"

namespace chunks::critical_chunks
{

class HeaderImage final : public Chunk
{
    static uint8_t _count;

    uint16_t _widthImage;
    uint16_t _heightImage;
    uint8_t _depth;
    uint8_t _colorType;
    uint8_t _compression;
    uint8_t _filter;
    uint8_t _interface;

    template<ErrorChunkHeaderImage error>
    static constexpr void checkErrorsHeaderImg(bool&& val)
    {
        checkError<ErrorChunkHeaderImage, error, 2>(std::forward<bool>(val));
    }

    static constexpr void check()
    {
        if (_count > 1)
            // errorsChunk<>();
                return;
        _count++;
    }


public:
    constexpr explicit HeaderImage(const uint32_t begin, const uint32_t end,
                          const char* const& data, const ModeChunk mode)
        : Chunk(begin, end, data, mode), _widthImage()
        , _heightImage(), _depth(), _colorType(), _compression(), _filter()
        , _interface()
    {
        check();
    }
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

    [[nodiscard]] constexpr uint16_t widthImage() const
    {
        return _widthImage;
    }
    [[nodiscard]] constexpr uint16_t heightImage() const
    {
        return _heightImage;
    }
    [[nodiscard]] constexpr uint8_t depth() const
    {
        return _depth;
    }
    [[nodiscard]] constexpr uint8_t colorType() const
    {
        return _colorType;
    }
    [[nodiscard]] constexpr uint8_t compression() const
    {
        return _compression;
    }
    [[nodiscard]] constexpr uint8_t filter() const
    {
        return _filter;
    }
    [[nodiscard]] constexpr uint8_t interface() const
    {
        return _interface;
    }
};


}

#endif //HERDERIMAGE_HPP
