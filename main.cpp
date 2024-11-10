
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

class FormatPng
{
private:
    static constexpr uint8_t SIZE_SIGNATURE = 8;
    static constexpr int8_t SIZE_DEFAULT_BLOCK_IN_CHUNK = 4;

    class Chunk
    {
    private:
        int64_t beginPos;
        int64_t endPos;
        int64_t length;

        char* descriptionChunk;
        char type[4];
        char crc[4];

    public:
        explicit Chunk(const int64_t len, const int64_t startPos)
            : beginPos(startPos), endPos(startPos + len + 4 * 2)
            , length(len), descriptionChunk(new char[len]), type{}, crc{} {}
        Chunk()
            : beginPos(1), endPos(1), length(1)
            , descriptionChunk(new char[length]), type{}, crc{} {}

        Chunk(const Chunk& other)
            : beginPos(other.beginPos), endPos(other.endPos), length(other.length)
            , descriptionChunk(new char[other.length]), type{}, crc{}
        {
            for (int i = 0; i < other.length; i++)
            {
                type[i] = other.type[i];
                crc[i] = other.crc[i];
            }

            std::copy(other.descriptionChunk,
                other.descriptionChunk + other.length, descriptionChunk);
        }
        Chunk(Chunk&& other) noexcept
            : beginPos(other.beginPos), endPos(other.endPos), length(other.length)
            , type{}, descriptionChunk(other.descriptionChunk), crc{}
        {
            for (int i = 0; i < other.length; i++)
            {
                type[i] = other.type[i];
                crc[i] = other.crc[i];
            }

            for (int i = 0; i < other.length; i++)
            {
                other.type[i] = 0;
                other.crc[i] = 0;
            }

            other.beginPos = 0;
            other.endPos = 0;
            other.descriptionChunk = nullptr;
            other.length = 0;
        }

        Chunk& operator=(const Chunk& other)
        {
            if (this == &other)
                return *this;

            for (int i = 0; i < other.length; i++)
            {
                type[i] = other.type[i];
                crc[i] = other.crc[i];
            }

            delete[] descriptionChunk;
            beginPos         = other.beginPos;
            endPos           = other.endPos;
            length           = other.length;
            descriptionChunk = new char[other.length];
            std::copy(other.descriptionChunk,
                other.descriptionChunk + other.length, descriptionChunk);
            return *this;
        }
        Chunk& operator=(Chunk&& other) noexcept
        {
            if (this == &other)
                return *this;

            delete[] descriptionChunk;
            beginPos         = other.beginPos;
            endPos           = other.endPos;
            length           = other.length;
            descriptionChunk = other.descriptionChunk;

            for (int i = 0; i < other.length; i++)
            {
                type[i] = other.type[i];
                crc[i] = other.crc[i];
            }

            for (int i = 0; i < other.length; i++)
            {
                other.type[i] = 0;
                other.crc[i] = 0;
            }

            other.descriptionChunk = nullptr;
            other.length = 0;
            other.beginPos = 0;
            other.endPos = 0;

            return *this;
        }

        ~Chunk()
        {
            if (!descriptionChunk)
                return;

            delete[] descriptionChunk;
        }

        enum class typeBlock : uint16_t
        {
            TYPE = 1,
            CRC,
            DESCRIPTION,
        };

        [[nodiscard]] int32_t getBeginPos() const
        {
            return beginPos;
        }
        [[nodiscard]] int32_t getEndPos() const
        {
            return endPos;
        }
        [[nodiscard]] int32_t getLength() const
        {
            return length;
        }

        [[nodiscard]] char* getDescriptionChunk() const
        {
            return descriptionChunk;
        }
        [[nodiscard]] char* getType()
        {
            return type;
        }
        [[nodiscard]] char* getCrc()
        {
            return crc;
        }

        static void templatePrint(const char* arr, const int32_t len)
        {
            for (auto i = arr; i < &arr[len]; i++)
                std::cout << std::hex << static_cast<int16_t>(*i) << ' ';
            std::cout << '\n';
        }

        void print(const typeBlock& type) const
        {
            switch (type)
            {
                case typeBlock::TYPE:
                {
                    std::cout << "Type (name) - ";
                    templatePrint(this->type, 4);
                    break;
                }
                case typeBlock::CRC:
                {
                    std::cout << "CRC - ";
                    templatePrint(crc, 4);
                    break;
                }
                case typeBlock::DESCRIPTION:
                {
                    std::cout << "data chunk - ";
                    templatePrint(descriptionChunk, length);
                    break;
                }
            }
        }
    };

    std::ifstream file;
    Chunk ihdr;
    Chunk srgb{};
    Chunk gama{};
    Chunk phys{};
    Chunk iend{};
    Chunk idat{};

    uint32_t sizeFile;
    uint32_t position;
    const char* nameFile;
    char signature[SIZE_SIGNATURE]{};
    char LENGTH_BLOCK_DATA_IN_CHUNK[SIZE_DEFAULT_BLOCK_IN_CHUNK]{};

    static int64_t convertCharInInt(const char length[4])
    {
        std::stringstream ss;
        ss << static_cast<int16_t>(length[0])
            << static_cast<int16_t>(length[1])
            << static_cast<int16_t>(length[2])
            << static_cast<int16_t>(length[3]);

        return std::stoll(ss.str());
    }

    int64_t calcLengthChunk()
    {
        file.read(LENGTH_BLOCK_DATA_IN_CHUNK, SIZE_DEFAULT_BLOCK_IN_CHUNK);
        file.seekg(position += SIZE_DEFAULT_BLOCK_IN_CHUNK);
        return convertCharInInt(LENGTH_BLOCK_DATA_IN_CHUNK);
    }

    void read(char* arr, const int32_t size)
    {
        file.read(arr, size);
        file.seekg(position += size);
    }

    Chunk createChunk()
    {
        Chunk chunk(calcLengthChunk(), position);
        read(chunk.getType(), SIZE_DEFAULT_BLOCK_IN_CHUNK);
        read(chunk.getDescriptionChunk(), chunk.getLength());
        read(chunk.getCrc(), SIZE_DEFAULT_BLOCK_IN_CHUNK);
        return chunk;
    }

    static void func(const Chunk& chunk)
    {
        std::cout << "\nLength data chunk: " << chunk.getLength() << '\n';
        chunk.print(Chunk::typeBlock::TYPE);
        chunk.print(Chunk::typeBlock::DESCRIPTION);
        chunk.print(Chunk::typeBlock::CRC);
    }

public:
    explicit FormatPng(const char* nameFile)
        : position(0), nameFile(nameFile)
    {
        file.open(nameFile, std::ifstream::binary | std::ifstream::in);
        if (!file.is_open())
            std::cerr << "Error!!\n";

        const std::streampos begin = file.tellg();
        file.seekg(0, std::ios::end);
        const std::streampos end = file.tellg();
        file.seekg(0);
        sizeFile = static_cast<uint32_t>(end - begin);

        read(signature, SIZE_SIGNATURE);
        ihdr = createChunk();
        srgb = createChunk();
        gama = createChunk();
        phys = createChunk();
        // idat = createChunk();
        // iend = createChunk();
    }
    ~FormatPng()
    {
        file.close();
    }

    [[nodiscard]] const char* getName() const
    {
        return nameFile;
    }
    [[nodiscard]] uint32_t getSize() const
    {
        return sizeFile;
    }

    [[nodiscard]] int16_t getNonAscii() const
    {
        return signature[0];
    }
    [[nodiscard]] const char* getNameFormat() const
    {
        return std::string{signature[1], signature[2], signature[3]}.c_str();
    }
    [[nodiscard]] const char* getDosStyleLineFeed() const
    {
        std::stringstream ss;
        ss << std::hex << static_cast<int16_t>(signature[4]) << ' '
            << static_cast<int16_t>(signature[5]);
        return std::string{ss.str()}.c_str();
    }
    [[nodiscard]] int16_t getStopsFileOutput() const
    {
        return signature[6];
    }
    [[nodiscard]] int16_t getUnixStyleLineFeed() const
    {
        return signature[7];
    }
    [[nodiscard]] const char* getSignatureFile() const
    {
        return signature;
    }

    enum typeChunk
    {
        IHDR = 1,
        SRGB,
        GAMA,
        PHYS,
        IDAT,
        IEND,
    };

    void ihdrPrint(const typeChunk type) const
    {
        switch (type)
        {
            case IHDR: func(ihdr); break;
            case SRGB: func(srgb); break;
            case GAMA: func(gama); break;
            case PHYS: func(phys); break;
            case IDAT: func(idat); break;
            case IEND: func(iend); break;
            default: std::cerr << "ERROR";
        }
    }

};


int main()
{
    const FormatPng fp("data.png");

    std::cout << "Name: " << fp.getName()
              << "\nSize: " << fp.getSize() << std::hex
              << "\nNon-ASCII: " << fp.getNonAscii()
              << "\nName format: " << fp.getNameFormat()
              << "\nDos-style line feed: " << fp.getDosStyleLineFeed()
              << "\nStops file output: " << fp.getStopsFileOutput()
              << "\nUnix-style line feed " << fp.getUnixStyleLineFeed() << "\n\n";

    fp.ihdrPrint(FormatPng::IHDR);
    fp.ihdrPrint(FormatPng::SRGB);
    fp.ihdrPrint(FormatPng::GAMA);
    fp.ihdrPrint(FormatPng::PHYS);
    // fp.ihdrPrint(FormatPng::IDAT);
    // fp.ihdrPrint(FormatPng::IEND);

    return 0;
}


