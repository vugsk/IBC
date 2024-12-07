//
// Created by ganch on 05.12.2024.
//

#ifndef DATAIMAGE_HPP
#define DATAIMAGE_HPP

namespace chunk::critical_chunk {

// char compressedDataBlocks[n]; n - count bytes
// char checkValue[4]{};

// Zlib header
//      1 byte -> Compression Method
//      4 bits -> from junior to senior ->
//      if equal 8, using algorithm Deflate
// uint8_t compressionMethod{}; // CM

//      4 bits -> parameter responsible for window size.
//      window size can find -> C = log2 W, W - window size, C - CINFO
// uint8_t compressionInfo{}; // CINFO

// uint32_t windowSize{}; // formula find -> C = log2 W - ?

//      1 byte -> Flags
//      2 bits
// uint8_t compressionLevel{};

//      1 bit
//      if he equal 1, per bytes Flags the dict should, must
// uint8_t presetDictionary{};

//      5 bits
//      number -> CMF * 256 + FLG -> was a multiple of 31
//uint8_t checkBitsForCmfAndFlg{};

//uint8_t dict[4]{}; // dict with uncompressed bytes if equal 1

// header block
// 1 bit -> installing in 1, if block end
// 2 bits -> information about the compression type:
//      1. 00 -> without compression
//      2. 01 -> compression with static Hoffmann
//      3. 10 -> compression with dynamic Hoffmann
//      4. 11 -> call error

// V=D−B+L -> formula for restore the original value
// D -> massive; B -> lower value from 3 raw table; L -> lower value from 1 raw table
// v = 256 -> end block, 286 and 287 -> extra ones

class DataImage {

};


} // chunk

#endif //DATAIMAGE_HPP
