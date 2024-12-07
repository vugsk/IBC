//
// Created by ganch on 22.11.2024.
//

#ifndef PNGFORMATREAD_HPP
#define PNGFORMATREAD_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>


class PngFormatRead {
private:
    class Chunk_test
    {
    protected:
        std::string _data;

        std::string substr(uint8_t begin, uint8_t end);
        static uint16_t convertStringInInt(const std::string& str);

    private:
        void calcLengthData();
        void calcTypeChunk();
        void calcCrc();

        std::string _name;
        uint32_t    _crc[4];
        uint32_t    _begin;
        uint32_t    _end;
        uint32_t    _size;
        uint16_t    _length;

        bool isCalcLength{};
        bool isCalcTypeChunk{};

    public:
        explicit Chunk_test(uint32_t begin, uint32_t end, const std::string& data);
        virtual ~Chunk_test() = default;

        [[nodiscard]] std::string name() const;
        [[nodiscard]] uint32_t begin() const;
        [[nodiscard]] uint32_t end() const;
        [[nodiscard]] uint32_t size() const;
        [[nodiscard]] uint16_t length() const;
    };
    struct Ihdr final : Chunk_test
    {
    private:
        uint16_t _widthImage;
        uint16_t _heightImage;
        uint8_t _depth;
        uint8_t _colorType;
        uint8_t _compression;
        uint8_t _filter;
        uint8_t _interface;

    public:
        Ihdr(uint32_t begin, uint32_t end, const std::string& data);
        ~Ihdr() override = default;

        [[nodiscard]] uint16_t widthImage() const;
        [[nodiscard]] uint16_t heightImage() const;
        [[nodiscard]] uint8_t depth() const;
        [[nodiscard]] uint8_t colorType() const;
        [[nodiscard]] uint8_t compression() const;
        [[nodiscard]] uint8_t filter() const;
        [[nodiscard]] uint8_t interface() const;
    };
    struct Plte final
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };
    struct Idat final
    {

        // char compressedDataBlocks[n]; n - count bytes
        char checkValue[4];

        // Zlib header
        // 1 byte -> Compression Method
        // 4 bits -> from junior to senior ->
        // if equal 8, using algorithm Deflate
        uint8_t compressionMethod; // CM
        // 4 bits -> parameter responsible for window size.
        // window size can find -> C = log2 W, W - window size, C - CINFO
        uint8_t compressionInfo; // CINFO
        uint32_t windowSize; // formula find -> C = log2 W - ?
        // 1 byte -> Flags
        // 2 bits
        uint8_t compressionLevel;
        // 1 bit
        // if he equal 1, per bytes Flags the dict should, must
        uint8_t presetDictionary;
        // 5 bits
        // number -> CMF * 256 + FLG -> was a multiple of 31
        uint8_t checkBitsForCmfAndFlg;
        uint8_t dict[4]; // dict with uncompressed bytes if equal 1

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


    };
    struct Iend final {};
    struct Gama final
    {
        uint32_t imageGamma;
    };
    struct Srgb final
    {
        uint8_t renderingIntent;
    };
    struct Phys final
    {
        uint32_t xAxis;
        uint32_t yAxis;
        uint8_t unitSpecifier;
    };
    struct Signature
    {
        char type[3];
        int8_t nonAscii;
        uint8_t dosStyle[2];
        uint8_t stopOutputFileDos;
        uint8_t unixStyle;
    };

    std::shared_ptr<Ihdr> _ihdr;
    std::shared_ptr<Idat> _idat;

    std::string _content;
    std::string _nameFile;
    Signature _signature{};

    uint32_t _size{};

    bool isInitSignature = false;

    void                                        readFile();
    std::vector<std::pair<uint16_t, uint8_t>> definedPositionChunks();
    void                                        definedSignature();

public:
    explicit PngFormatRead(const char* nameFile);
    ~PngFormatRead() = default;

    [[nodiscard]] std::shared_ptr<Ihdr> ihdr() const;
    [[nodiscard]] std::string nameFile() const;
    [[nodiscard]] Signature signature() const;
    [[nodiscard]] uint32_t size() const;
};



#endif //PNGFORMATREAD_HPP
