
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>

class chunk
{
private:
    int32_t beginPos;
    int32_t endPos;
    int32_t length;

    char* descriptionChunk;
    char type[4];
    char crc[4];

public:
    explicit chunk(const int32_t len, const int32_t startPos)
        : beginPos(startPos), endPos(startPos + len + 4 * 2)
        , length(len), type{}, crc{}
    {
        descriptionChunk = new char[len];
    }

    chunk(const chunk& other)
        : beginPos(other.beginPos), endPos(other.endPos), length(other.length)
        , type{}, descriptionChunk(new char[other.length]), crc{}
    {
        for (int i = 0; i < other.length; i++)
        {
            type[i] = other.type[i];
            crc[i] = other.crc[i];
        }

        std::copy(other.descriptionChunk,
            other.descriptionChunk + other.length, descriptionChunk);
    }
    chunk(chunk&& other) noexcept
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

    chunk& operator=(const chunk& other)
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
    chunk& operator=(chunk&& other) noexcept
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

    ~chunk()
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

class FormatPng
{

};

static int32_t size_file(std::ifstream& file)
{
    const std::streampos begin = file.tellg();
    file.seekg (0, std::ios::end);
    const std::streampos end = file.tellg();
    file.seekg(0);
    return static_cast<int32_t>(end - begin - 20);
}

static int32_t convertCharInInt(char length[4])
{
    return std::stoi(std::to_string(length[0]) +
                        std::to_string(length[1]) +
                        std::to_string(length[2]) +
                        std::to_string(length[3]));
}

static int32_t calcLengthChunk(std::ifstream& file, int32_t& pos)
{
    static constexpr int8_t size = 4;
    char len[size];
    file.seekg(pos);
    file.read(len, size);
    file.seekg(pos += size);
    return convertCharInInt(len);
}

void read(std::ifstream& file, char* arr, int32_t& pos, const int32_t size)
{
    file.read(arr, size);
    file.seekg(pos += size);
}

chunk createChunk(std::ifstream& file, int32_t& pos)
{
    chunk header(calcLengthChunk(file, pos), pos);

    read(file, header.getType(), pos, 4);
    read(file, header.getDescriptionChunk(), pos, header.getLength());
    read(file, header.getCrc(), pos, 4);

    return header;
}


int main()
{
    // signator PNG
    int pos = 8;
    char signator[8];


    std::ifstream img("data.png", std::ifstream::binary | std::ifstream::in);

    if (!img.is_open())
        return -1;

    std::cout << "size img - " << size_file(img) << '\n';

    img.read(signator, pos);

    const chunk header = createChunk(img, pos);
    header.print(chunk::typeBlock::TYPE);
    header.print(chunk::typeBlock::DESCRIPTION);
    header.print(chunk::typeBlock::CRC);
    std::cout << "------\n";
    const chunk srgb = createChunk(img, pos);
    srgb.print(chunk::typeBlock::TYPE);
    srgb.print(chunk::typeBlock::DESCRIPTION);
    srgb.print(chunk::typeBlock::CRC);

    std::cout << std::dec << "pos " << pos << '\n';

    img.close();

    return 0;
}


