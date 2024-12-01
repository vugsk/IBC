cmake_minimum_required(VERSION 3.29)
project(BinirisationImgCpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(BinirasionImgCpp src/main.cpp
    src/PngFormatLib/PngFormat.cpp
    src/PngFormatLib/PngFormat.hpp
    src/PngFormatLib/Chunk.hpp
    src/PngFormatLib/ErrorInfo.hpp
    src/PngFormatLib/HeaderImage.cpp
    src/PngFormatLib/HeaderImage.hpp
    src/PngFormatLib/GeneralFunction.hpp
)
