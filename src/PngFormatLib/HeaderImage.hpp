//
// Created by ganch on 26.11.2024.
//

#ifndef HERDERIMAGE_HPP
#define HERDERIMAGE_HPP

#include <cstdint>

#include "Chunk.hpp"

namespace chunks::critical_chunks
{

enum class ErrorChunkHeaderImage : uint8_t
{
    NONE = 0,


};

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
                          const char* const& data, const ModeChunk mode,
                          const TypesErrorsChunks& type_error)
        : Chunk(begin, end, data, mode, type_error), _widthImage(0)
        , _heightImage(0), _depth(0), _colorType(0), _compression(0), _filter(0)
        , _interface(0), _isInit(false)
    {
        _isCanMultiChucks = true;
    }
    constexpr ~HeaderImage() override = default;

};


}

#endif //HERDERIMAGE_HPP
