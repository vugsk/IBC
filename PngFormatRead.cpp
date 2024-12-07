//
// Created by ganch on 22.11.2024.
//

#include "PngFormatRead.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>

static constexpr uint8_t SIZE_SIGNATURE = 8;
static constexpr uint8_t SIZE_DEFAULT_BLOCK_FOR_CHUNK = 4;

enum typeChunk : uint8_t
{
    DEFAULT = 0,
    IHDR,
    IDAT,
    IEND,
};

static const std::array arr
{
    std::make_pair<std::string, typeChunk>(std::string("IHDR"), IHDR),
    std::make_pair<std::string, typeChunk>(std::string("IDAT"), IDAT),
    std::make_pair<std::string, typeChunk>(std::string("IEND"), IEND),
    std::make_pair<std::string, typeChunk>(std::string("sRGB"), DEFAULT),
    std::make_pair<std::string, typeChunk>(std::string("gAMA"), DEFAULT),
    std::make_pair<std::string, typeChunk>(std::string("pHYs"), DEFAULT),
};

// Function from class Chunk
// public
std::string PngFormatRead::Chunk_test::substr(const uint8_t begin,
    const uint8_t end)
{
    std::string str = _data.substr(begin, end);
    _data.erase(begin, end);
    return str;
}

uint16_t PngFormatRead::Chunk_test::convertStringInInt(const std::string& str)
{
    std::stringstream ss;
    for (const char& i : str)
        if (static_cast<uint8_t>(i) != 0)
            ss << static_cast<int16_t>(i);
    return static_cast<uint8_t>(std::stoll(ss.str()));
}

// private
void PngFormatRead::Chunk_test::calcLengthData()
{
    if (isCalcTypeChunk || isCalcLength)
    {
        std::cerr << "ERROR: order is not observe call function";
        throw;
    }

    _length      = convertStringInInt(substr(0, 4));
    isCalcLength = true;
}

void PngFormatRead::Chunk_test::calcTypeChunk()
{
    if (isCalcTypeChunk || !isCalcLength)
    {
        std::cerr << "ERROR: order is not observe call function";
        throw;
    }

    _name           = substr(0, 4);
    isCalcTypeChunk = true;
}

void PngFormatRead::Chunk_test::calcCrc()
{
    static const std::string crc = substr(_data.size() - 4, _data.size());
    for (uint8_t i = 0; i < static_cast<uint8_t>(crc.size()); i++)
        _crc[i]    = static_cast<int32_t>(crc[i]);
}

PngFormatRead::Chunk_test::Chunk_test(const uint32_t begin, const uint32_t end,
    const std::string& data): _data(data), _crc{}, _begin(begin)
                          , _end(end), _size(data.size()), _length(0)
{
    calcLengthData();
    calcTypeChunk();
    calcCrc();
}

std::string PngFormatRead::Chunk_test::name() const
{
    return _name;
}
uint32_t PngFormatRead::Chunk_test::begin() const
{
    return _begin;
}
uint32_t PngFormatRead::Chunk_test::end() const
{
    return _end;
}
uint32_t PngFormatRead::Chunk_test::size() const
{
    return _size;
}
uint16_t PngFormatRead::Chunk_test::length() const
{
    return _length;
}

// Function from class Ihdr
// public
PngFormatRead::Ihdr::Ihdr(const uint32_t begin, const uint32_t end,
    const std::string& data): Chunk_test(begin, end, data)
                          , _widthImage(convertStringInInt(substr(0, 4)))
                          , _heightImage(convertStringInInt(substr(0, 4)))
                          , _depth(_data[0]), _colorType(_data[1]), _compression(_data[2])
                          , _filter(_data[3]), _interface(_data[4])
{
    _data.clear();
}

uint16_t PngFormatRead::Ihdr::widthImage() const
{
    return _widthImage;
}
uint16_t PngFormatRead::Ihdr::heightImage() const
{
    return _heightImage;
}
uint8_t PngFormatRead::Ihdr::depth() const
{
    return _depth;
}
uint8_t PngFormatRead::Ihdr::colorType() const
{
    return _colorType;
}
uint8_t PngFormatRead::Ihdr::compression() const
{
    return _compression;
}
uint8_t PngFormatRead::Ihdr::filter() const
{
    return _filter;
}
uint8_t PngFormatRead::Ihdr::interface() const
{
    return _interface;
}

// Function from main class PngFormatRead
// private
void PngFormatRead::readFile()
{
    std::ifstream _file(_nameFile, std::ios::in | std::ios::binary);
    if (!_file.is_open())
    {
        std::cerr << "ERROR: is impossible to open file!!!\n";
        throw;
    }

    _file.seekg(0, std::ios::end);
    const std::streampos end = _file.tellg();
    _file.seekg(0, std::ios::beg);
    _size = end;

    _content = std::string((std::istreambuf_iterator(_file)),
                           std::istreambuf_iterator<char>());
    _file.close();
}

std::vector<std::pair<uint16_t, uint8_t>> PngFormatRead::definedPositionChunks()
{
    std::vector<std::pair<uint16_t, uint8_t>> res;
    for (const auto& [fst, snd] : arr)
        res.emplace_back(_content.find(fst), snd);
    std::ranges::sort(res, [](const auto& l, const auto& r) -> bool
    {
        return l.first < r.first;
    });
    return res;
}

void PngFormatRead::definedSignature()
{
    if (isInitSignature)
    {
        std::cerr << "ERROR: init signature!!!\n";
        throw;
    }

    isInitSignature = true;

    _signature.nonAscii = _content[0];

    _signature.type[0] = _content[1];
    _signature.type[1] = _content[2];
    _signature.type[2] = _content[3];

    _signature.dosStyle[0] = _content[4];
    _signature.dosStyle[1] = _content[5];

    _signature.stopOutputFileDos = _content[6];
    _signature.unixStyle         = _content[7];
}

// public
PngFormatRead::PngFormatRead(const char* nameFile): _nameFile(nameFile)
{
    readFile();
    definedSignature();
    const auto& ii = definedPositionChunks();
    for (uint8_t i = 0; i < static_cast<uint8_t>(ii.size()); i++)
    {
        switch (ii[i].second)
        {
            case IHDR:
                _ihdr = std::make_unique<Ihdr>(ii[i].first - 4, ii[i + 1].first - 4,
                    _content.substr(ii[i].first - 4, ii[i + 1].first - 4));
            case IDAT:

            default: return;
        }
    }
}

std::shared_ptr<PngFormatRead::Ihdr> PngFormatRead::ihdr() const
{
    return _ihdr;
}
std::string PngFormatRead::nameFile() const
{
    return _nameFile;
}
PngFormatRead::Signature PngFormatRead::signature() const
{
    return _signature;
}
uint32_t PngFormatRead::size() const
{
    return _size;
}




