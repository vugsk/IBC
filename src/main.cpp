
#include <iostream>
#include <fstream>
#include <ranges>
#include <utility>

#include "PngFormatLib/Chunk.hpp"

using namespace chunks;

template<auto number> requires(std::is_integral_v<decltype(number)>)
struct func_defined_type
{
    constexpr auto operator()() const noexcept
    {
        if constexpr (number < INT8_MAX && number > INT8_MIN)
            return static_cast<int8_t>(number);
        else if constexpr (number < UINT8_MAX && number > 0)
            return static_cast<uint8_t>(number);
        else if constexpr (number < INT16_MAX && number > INT16_MIN)
            return static_cast<int16_t>(number);
        else if constexpr (number < UINT16_MAX && number > 0)
            return static_cast<uint16_t>(number);
        else if constexpr (number < INT32_MAX && number > INT32_MIN)
            return static_cast<int32_t>(number);
        else if constexpr (number < UINT32_MAX && number > 0)
            return static_cast<uint32_t>(number);
        else if constexpr (number < INT64_MAX && number > INT64_MIN)
            return static_cast<int64_t>(number);
        else if constexpr (number < UINT64_MAX && number > 0)
            return static_cast<uint64_t>(number);
    };
};

template<auto number> requires(std::is_integral_v<decltype(number)>)
inline constexpr auto func_defined_type_t = func_defined_type<number>();


int main()
{
    static constexpr auto nameFileForRead = "data.png";

    std::ifstream file(nameFileForRead, std::ios::in | std::ios::binary);

    if (!file.is_open())
        return -1;

    file.seekg(8);

    const auto name = new char[27];
    file.read(name, 26);
    name[26] = '\0';
    file.close();

    const Chunk chunk(8, 33, name, ModeChunk::READ);

    std::cout << "LEN -> " << chunk.length() << '\n'
                << "Name -> " << chunk.name() << '\n'
                << "Crc -> " << chunk.crc() << '\n'
                << "Size -> " << chunk.size() << '\n'
                << "Mode -> " << chunk.toStringMode() << '\n';

    delete[] name;

    // const PngFormatRead pfr(nameFileForRead);

    // std::cout << "IHDR chunk -> " << ihdr.begin()
    //             << ' ' << ihdr.end()
    //             << ' ' << ihdr.size()
    //             << ' ' << ihdr.length()
    //             << ' ' << ihdr.name()
    //             << "\nCRC -> " << std::hex
    //                            << ihdr.crc()[0]
    //                     << ' ' << ihdr.crc()[1]
    //                     << ' ' << ihdr.crc()[2]
    //                     << ' ' << ihdr.crc()[3]
    //             << std::dec
    //             << "\nData -> " << ihdr.heightImage()
    //             << ' ' << ihdr.widthImage()
    //             << ' ' << static_cast<uint16_t>(ihdr.depth())
    //             << ' ' << static_cast<uint16_t>(ihdr.colorType())
    //             << ' ' << static_cast<uint16_t>(ihdr.compression())
    //             << ' ' << static_cast<uint16_t>(ihdr.filter())
    //             << ' ' << static_cast<uint16_t>(ihdr.interface()) << '\n';

    // for (const auto& chunk : chunktest)
    //     std::cout << chunk.begin1() << ' ' << chunk.end1() << '\n';


    return 0;
}


