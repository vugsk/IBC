
#include <fstream>
#include <iostream>

#include "PngFormatLib/Chunk.hpp"
#include "PngFormatLib/HeaderImage.hpp"
#include "PngFormatLib/Signature.hpp"

int main()
{
    static constexpr auto NAME_FILE_FOR_READ = "data.png";

    if (std::ifstream file(NAME_FILE_FOR_READ, std::ios::in | std::ios::binary);
        file.is_open())
    {

        const auto sig = new char[9];
        file.read(sig, 8);
        sig[8] = '\0';

        file.seekg(8);
        const auto name = new char[27];
        file.read(name, 26);
        name[26] = '\0';
        file.close();

        const signature::Signature* sign = signature::Signature::create(0, 7, sig);

        std::cout << sign->getType() << '\n';

        delete sign;

        const chunks::critical_chunks::HeaderImage*
        chunk_header_img = chunks::critical_chunks::HeaderImage::create(
            8, 33, name, chunks::ModeChunk::READ);

        std::cout << "LEN -> " << chunk_header_img->length() << '\n'
                    << "Name -> " << chunk_header_img->name() << '\n'
                    << "Crc -> " << chunk_header_img->crc() << '\n'
                    << "Size -> " << chunk_header_img->size() << '\n'
                    << "Mode -> " << chunk_header_img->mode() << '\n'
                    << "Width -> " << chunk_header_img->widthImage() << '\n'
                    << "Height -> " << chunk_header_img->heightImage() << std::hex << '\n'
                    << "Depth -> " << static_cast<uint16_t>(chunk_header_img->depth()) << '\n'
                    << "Color type -> " << static_cast<uint16_t>(chunk_header_img->colorType()) << '\n'
                    << "Compression -> " << static_cast<uint16_t>(chunk_header_img->compression()) << '\n'
                    << "Filter -> " << static_cast<uint16_t>(chunk_header_img->filter()) << '\n'
                    << "Interface -> " << static_cast<uint16_t>(chunk_header_img->interface()) << std::dec << '\n';
        delete chunk_header_img;

        return 0;
    }
    return -1;
}


