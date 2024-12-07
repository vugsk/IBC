cmake_minimum_required(VERSION 3.29)
project(BinirisationImgCpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(BinirasionImgCpp src/main.cpp
    src/PngFormatLib/PngFormat.cpp
    src/PngFormatLib/PngFormat.hpp
    src/PngFormatLib/Chunk.hpp
    src/PngFormatLib/ErrorInformationChunks.hpp
    src/PngFormatLib/HeaderImage.cpp
    src/PngFormatLib/HeaderImage.hpp
    src/PngFormatLib/GeneralFunction.hpp
    src/PngFormatLib/Plte.cpp
    src/PngFormatLib/Plte.hpp
    src/PngFormatLib/EndImage.cpp
    src/PngFormatLib/EndImage.hpp
    src/PngFormatLib/DataImage.cpp
    src/PngFormatLib/DataImage.hpp
    src/PngFormatLib/Signature.cpp
    src/PngFormatLib/Signature.hpp
    src/PngFormatLib/ErrorObject.hpp
)
