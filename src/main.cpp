
#include <iostream>

#include "PngFormatLib/PngFormat.hpp"



int main()
{
    static constexpr auto NAME_FILE_FOR_READ = "data.png";

    const png_formation::PngFormat<png_formation::ModeFile::READ> png(NAME_FILE_FOR_READ);

    std::cout << "Size -> " << png.size() << '\n'
                << "Name -> " << png.fileName() << '\n'
                << "Sig type -> " << png.signature1()->getType() << '\n';

    return 0;
}


