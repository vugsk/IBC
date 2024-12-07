//
// Created by ganch on 25.11.2024.
//

#ifndef PNGFORMAT_HPP
#define PNGFORMAT_HPP

#include <cstdint>

#include "DataImage.hpp"
#include "EndImage.hpp"
#include "HeaderImage.hpp"
#include "Signature.hpp"

// struct DopChunk
// {
// private:
//     struct Gama final : Chunk
//     {
//         uint32_t imageGamma{};
//     };
//     struct Srgb final : Chunk
//     {
//         uint8_t renderingIntent{};
//     };
//     struct Phys final : Chunk
//     {
//         uint32_t xAxis{};
//         uint32_t yAxis{};
//         uint8_t unitSpecifier{};
//     };
//
// public:
//     Gama gama;
//     Srgb srgb;
//     Phys phys;
// };

class PngFormat
{
    chunks::critical_chunks::HeaderImage _headerImage;
    chunk::critical_chunk::DataImage _dataImage;
    chunks::critical_chunks::EndImage _endImage;

    signature::Signature _signature;
    const char* _fileName{};
    uint32_t _size{};

public:
};



#endif //PNGFORMAT_HPP
