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
    uint16_t _widthImage;
    uint16_t _heightImage;
    uint8_t _depth;
    uint8_t _colorType;
    uint8_t _compression;
    uint8_t _filter;
    uint8_t _interface;

    bool _isInit;
public:
    constexpr explicit HeaderImage(const uint32_t begin, const uint32_t end,
                          const char* const& data, const ModeChunk mode)
        : Chunk(begin, end, data, mode), _widthImage()
        , _heightImage(), _depth(), _colorType(), _compression(), _filter()
        , _interface(), _isInit(true) {}
    constexpr ~HeaderImage() override = default;



    [[nodiscard]] uint16_t widthImage() const
    {
        return _widthImage;
    }
    [[nodiscard]] uint16_t heightImage() const
    {
        return _heightImage;
    }
    [[nodiscard]] uint8_t depth() const
    {
        return _depth;
    }
    [[nodiscard]] uint8_t colorType() const
    {
        return _colorType;
    }
    [[nodiscard]] uint8_t compression() const
    {
        return _compression;
    }
    [[nodiscard]] uint8_t filter() const
    {
        return _filter;
    }
    [[nodiscard]] uint8_t interface() const
    {
        return _interface;
    }
};


}

#endif //HERDERIMAGE_HPP
