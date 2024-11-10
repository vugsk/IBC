
#include <cstdint>
#include <fstream>
#include <iostream>




struct chunk
{
    explicit chunk(const int32_t len, const int16_t startPos)
        : beginPos(startPos), endPos(startPos + len + 4 * 2)
        , lenght(len), type{}, crc{}
    {
        descriptionChunk = new char[len];
    }

    chunk(const chunk& other)
        : beginPos(other.beginPos)
      , endPos(other.endPos)
      , lenght(other.lenght)
      , descriptionChunk(new char[other.lenght])
    {
        for (int i = 0; i < 4; i++)
        {
            type[i] = other.type[i];
            crc[i] = other.crc[i];
        }

        std::copy(other.descriptionChunk,
            other.descriptionChunk + other.lenght, descriptionChunk);
    }

    chunk(chunk&& other) noexcept
        : beginPos(other.beginPos)
      , endPos(other.endPos)
      , lenght(other.lenght)
      , descriptionChunk(other.descriptionChunk)
    {
        for (int i = 0; i < 4; i++)
        {
            type[i] = other.type[i];
            crc[i] = other.crc[i];
        }

        other.descriptionChunk = nullptr;
        other.lenght = 0;
        other.crc = {};
        other.type = {};
        other.beginPos = 0;
        other.endPos = 0;
    }

    chunk& operator=(const chunk& other)
    {
        if (this == &other)
            return *this;

        for (int i = 0; i < 4; i++)
        {
            type[i] = other.type[i];
            crc[i] = other.crc[i];
        }

        delete[] descriptionChunk;
        beginPos         = other.beginPos;
        endPos           = other.endPos;
        lenght           = other.lenght;
        descriptionChunk = new char[other.lenght];
        std::copy(other.descriptionChunk,
            other.descriptionChunk + other.lenght, descriptionChunk);
        return *this;
    }

    chunk& operator=(chunk&& other) noexcept
    {
        if (this == &other)
            return *this;

        for (int i = 0; i < 4; i++)
        {
            type[i] = other.type[i];
            crc[i] = other.crc[i];
        }

        delete[] descriptionChunk;
        beginPos         = other.beginPos;
        endPos           = other.endPos;
        lenght           = other.lenght;
        descriptionChunk = other.descriptionChunk;
        return *this;
    }

    int32_t beginPos;
    int32_t endPos;
    int32_t lenght;
    char type[4];
    char* descriptionChunk;
    char crc[4];

    ~chunk()
    {
        delete[] descriptionChunk;
    }
};


static int32_t size_file(std::ifstream& file)
{
    const std::streampos begin = file.tellg();
    file.seekg (0, std::ios::end);
    const std::streampos end = file.tellg();
    file.seekg(0);
    return static_cast<int32_t>(end - begin - 20);
}

static int32_t convertCharInInt(char lenght[4])
{
    return std::stoi(std::to_string(lenght[0]) +
                        std::to_string(lenght[1]) +
                        std::to_string(lenght[2]) +
                        std::to_string(lenght[3]));
}

static int32_t calcLengthChunk(std::ifstream& file, int& pos)
{
    static constexpr int8_t size = 4;
    char len[size];
    file.seekg(pos);
    file.read(len, size);
    file.seekg(pos += size);
    return convertCharInInt(len);
}

chunk createChunk(std::ifstream& file, int pos)
{
    chunk header(calcLengthChunk(file, pos), pos);

    std::cout << "Length header - " << header.lenght << '\n';

    file.read(header.type, 4);
    file.seekg(16);
    std::cout << "Type (name) - ";
    for (const auto i : header.type)
        std::cout << std::hex << static_cast<int16_t>(i) << ' ';
    std::cout << '\n';

    file.read(header.descriptionChunk, header.lenght);
    file.seekg(16 + header.lenght);

    std::cout << "data chunk - ";
    for (int i = 0; i < header.lenght; i++)
        std::cout << std::hex << static_cast<int16_t>(header.descriptionChunk[i]) << ' ';
    std::cout << '\n';

    file.read(header.crc, 4);
    file.seekg(20 + header.lenght);

    std::cout << "CRC - ";
    for (const auto i : header.crc)
        std::cout << std::hex << static_cast<int16_t>(i) << ' ';
    std::cout << '\n';

    return header;
}

int main()
{
    // signator PNG
    int pos = 8;
    char signator[8];
    char lenght[4];


    std::ifstream img("data.png", std::ifstream::binary | std::ifstream::in);

    if (!img.is_open())
        return -1;

    std::cout << "size img - " << size_file(img) << '\n';

    img.read(signator, pos);

    chunk header = createChunk(img, pos);

    std::cout << "Type (name) - ";
    for (const auto i : header.type)
        std::cout << std::hex << static_cast<int16_t>(i) << ' ';
    std::cout << '\n';
    
    std::cout << "data chunk - ";
    for (int i = 0; i < header.lenght; i++)
        std::cout << std::hex << static_cast<int16_t>(header.descriptionChunk[i]) << ' ';
    std::cout << '\n';

    std::cout << "CRC - ";
    for (const auto i : header.crc)
        std::cout << std::hex << static_cast<int16_t>(i) << ' ';
    std::cout << '\n';

    img.close();

    for (const auto i : signator)
        std::cout << std::hex << static_cast<int16_t>(i) << ' ';
    std::cout << '\n';

    return 0;
}


