//
// Created by ganch on 25.11.2024.
//

#ifndef PNGFORMAT_HPP
#define PNGFORMAT_HPP

#include <cstdint>
#include <string>
#include <vector>

struct Png
{
private:
    struct Signature
    {
        char type[3];
        int8_t nonAscii;
        uint8_t dosStyle[2];
        uint8_t stopOutputFileDos;
        uint8_t unixStyle;
    };
    // struct MainChunk
    // {
    // private:
    //     struct Indr final : Chunk
    // {
    //     uint16_t _widthImage{};
    //     uint16_t _heightImage{};
    //     uint8_t _depth{};
    //     uint8_t _colorType{};
    //     uint8_t _compression{};
    //     uint8_t _filter{};
    //     uint8_t _interface{};
    // };
    //     struct Plte final : Chunk
    //     {
    //         uint8_t red{};
    //         uint8_t green{};
    //         uint8_t blue{};
    //     };
    //     struct Idat final : Chunk
    //     {
    //
    //         // char compressedDataBlocks[n]; n - count bytes
    //         char checkValue[4]{};
    //
    //         // Zlib header
    //         // 1 byte -> Compression Method
    //         // 4 bits -> from junior to senior ->
    //         // if equal 8, using algorithm Deflate
    //         uint8_t compressionMethod{}; // CM
    //         // 4 bits -> parameter responsible for window size.
    //         // window size can find -> C = log2 W, W - window size, C - CINFO
    //         uint8_t compressionInfo{}; // CINFO
    //         uint32_t windowSize{}; // formula find -> C = log2 W - ?
    //         // 1 byte -> Flags
    //         // 2 bits
    //         uint8_t compressionLevel{};
    //         // 1 bit
    //         // if he equal 1, per bytes Flags the dict should, must
    //         uint8_t presetDictionary{};
    //         // 5 bits
    //         // number -> CMF * 256 + FLG -> was a multiple of 31
    //         uint8_t checkBitsForCmfAndFlg{};
    //         uint8_t dict[4]{}; // dict with uncompressed bytes if equal 1
    //
    //         // header block
    //         // 1 bit -> installing in 1, if block end
    //         // 2 bits -> information about the compression type:
    //         //      1. 00 -> without compression
    //         //      2. 01 -> compression with static Hoffmann
    //         //      3. 10 -> compression with dynamic Hoffmann
    //         //      4. 11 -> call error
    //
    //         // V=D−B+L -> formula for restore the original value
    //         // D -> massive; B -> lower value from 3 raw table; L -> lower value from 1 raw table
    //         // v = 256 -> end block, 286 and 287 -> extra ones
    //
    //
    //     };
    //     struct Iend final : Chunk
    //     {
    //
    //     };
    //
    // public:
    //     Indr indr;
    //     Plte plte;
    //     Idat idat;
    //     Iend iend;
    // };

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

public:
    std::vector<std::string> chunks;
    // MainChunk mainChunk;
    // DopChunk dopChunk;
    std::string nameFile;
    Signature signature;
    uint32_t size;
};

class PngFormat
{
private:
public:
};



#endif //PNGFORMAT_HPP
