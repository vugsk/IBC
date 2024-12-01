
#include <fstream>
#include <iostream>
#include <vector>

#include "PngFormatLib/Chunk.hpp"
#include "PngFormatLib/HeaderImage.hpp"

int main()
{
    static constexpr auto NAME_FILE_FOR_READ = "data.png";

    if (std::ifstream file(NAME_FILE_FOR_READ, std::ios::in | std::ios::binary);
        file.is_open())
    {
        file.seekg(8);

        const auto name = new char[27];
        file.read(name, 26);
        name[26] = '\0';

        const chunks::critical_chunks::HeaderImage
                chunk_header_img(8, 33, name, chunks::ModeChunk::READ);

        std::cout << "LEN -> " << chunk_header_img.length() << '\n'
                    << "Name -> " << chunk_header_img.name() << '\n'
                    << "Crc -> " << chunk_header_img.crc() << '\n'
                    << "Size -> " << chunk_header_img.size() << '\n'
                    << "Mode -> " << chunk_header_img.mode() << '\n';

        file.close();

        return 0;
    }
    return -1;
}


