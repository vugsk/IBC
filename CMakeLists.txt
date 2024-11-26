cmake_minimum_required(VERSION 3.29)
project(BinirisationImgCpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(BinirisationImgCpp src/main.cpp
    # PngFormatRead.cpp
    # PngFormatRead.hpp
    src/PngFormatLib/PngFormat.cpp
    src/PngFormatLib/PngFormat.hpp
    src/PngFormatLib/Chunk.hpp
    src/PngFormatLib/ErrorInfo.hpp
    src/PngFormatLib/HerderImage.cpp
    src/PngFormatLib/HerderImage.hpp)
