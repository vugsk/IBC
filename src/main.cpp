
#include <fstream>
#include <iostream>

#include "PngFormatLib/Chunk.hpp"

using namespace chunks;

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
                << "Mode -> " << chunk.testMode() << '\n';

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


