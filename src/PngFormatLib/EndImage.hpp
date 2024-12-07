//
// Created by ganch on 05.12.2024.
//

#ifndef ENDIMAGE_HPP
#define ENDIMAGE_HPP

#include "Chunk.hpp"


namespace chunks::critical_chunks
{

class EndImage final : public Chunk{
public:
    constexpr EndImage(const uint32_t begin, const uint32_t end,
                        const char* const& data, const ModeChunk& mode)
        : Chunk(begin, end, data, mode) {}
    constexpr ~EndImage() override = default;
    EndImage(const EndImage& other) = default;
    EndImage(EndImage&& other) noexcept
        : Chunk(std::move(other)) {}

    EndImage& operator=(const EndImage& other)
    {
        if (this == &other)
            return *this;
        Chunk::operator =(other);
        return *this;
    }
    EndImage& operator=(EndImage&& other) noexcept
    {
        if (this == &other)
            return *this;
        Chunk::operator =(std::move(other));
        return *this;
    }
};

} // chunk

#endif //ENDIMAGE_HPP
