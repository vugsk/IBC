//
// Created by ganch on 26.11.2024.
//

#ifndef ERRORINFO_HPP
#define ERRORINFO_HPP

#include <array>
#include <cstdint>

#include "ErrorObject.hpp"

namespace chunks
{

namespace critical_chunks
{

enum class ErrorChunkHeaderImage : uint8_t
{
    NONE = 0,

    IT_SHOULD_NOT_BE_INITIALIZED_AGAIN,

    CAN_NOT_REDEFINE_WIDTH_IMAGE,
    CAN_NOT_REDEFINE_HEIGHT_IMAGE,

    THE_WIDTH_IMAGE_MUST_NOT_BE_LESS_THAN_ZERO,
    THE_HEIGHT_IMAGE_MUST_NOT_BE_LESS_THAN_ZERO,

};

}

enum class ErrorsChunks : uint8_t
{
    NONE = 0, // if all is good

    CAN_NOT_REDEFINE_LENGTH_BLOCK,  // implemented in class Chunk
    CAN_NOT_REDEFINE_CRC_BLOCK,     // implemented in class Chunk
    CAN_NOT_REDEFINE_DATA_BLOCK,
    CHUNK_DAMAGED,                  // implemented in class Chunk

    CAN_NOT_USING_FUNCTIONS_NONE,               // implemented in class Chunk
    CAN_NOT_USING_FUNCTIONS_WRITES,             // implemented in class Chunk
    CAN_NOT_USING_FUNCTIONS_READS,              // implemented in class Chunk
    CAN_NOT_USING_FUNCTIONS_READS_OR_WRITES,    // implemented in class Chunk

    CAN_NOT_CONVERTING_STRING_IN_INTEGER,   // implemented in class Chunk
    CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING,  // implemented in class Chunk
    CAN_NOT_WAS_CHUNK_SHORT_THAN_12_BYTES,  // implemented in class Chunk
    THE_LENGTH_MUST_NOT_BE_LESS_THAN_ZERO,  // implemented in class Chunk
};

static constexpr std::array GENERAL_ERRORS_CHUNK
{
    // Errors -> redefine blocks
    error_information::ErrorInformationChunks(ErrorsChunks::CAN_NOT_REDEFINE_LENGTH_BLOCK,
              "Can`t redefine length block!!!"),
    error_information::ErrorInformationChunks(ErrorsChunks::CAN_NOT_REDEFINE_CRC_BLOCK,
              "Can`t redefine CRC block!!!"),
    error_information::ErrorInformationChunks(ErrorsChunks::CAN_NOT_REDEFINE_DATA_BLOCK,
              "Can`t redefine data block!!!"),
    error_information::ErrorInformationChunks(ErrorsChunks::CHUNK_DAMAGED,
              "Chunk to damaged!!!"),

    // Error -> mode chunk
    error_information::ErrorInformationChunks(ErrorsChunks::CAN_NOT_USING_FUNCTIONS_NONE,
              "Can`t using functions none!!!"),
    error_information::ErrorInformationChunks(ErrorsChunks::CAN_NOT_USING_FUNCTIONS_READS,
              "Can`t using functions reads!!!"),
    error_information::ErrorInformationChunks(ErrorsChunks::CAN_NOT_USING_FUNCTIONS_WRITES,
              "Can`t using functions writes!!!"),
    error_information::ErrorInformationChunks(ErrorsChunks::CAN_NOT_USING_FUNCTIONS_READS_OR_WRITES,
              "Can't use functions to read or write!!!"),

    // Error -> internal function class Chunk
    error_information::ErrorInformationChunks(ErrorsChunks::CAN_NOT_CONVERTING_STRING_IN_INTEGER,
              "Can't convert string to integer!!!"),
    error_information::ErrorInformationChunks(ErrorsChunks::CAN_NOT_WAS_CHUNK_SHORT_THAN_12_BYTES,
              "Can't be chunk shorter than 12 bytes!!!"),
    error_information::ErrorInformationChunks(ErrorsChunks::CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING,
              "Can't crash string on substring!!!"),
    error_information::ErrorInformationChunks(ErrorsChunks::THE_LENGTH_MUST_NOT_BE_LESS_THAN_ZERO,
              "The length mustn't be less than zero!!!"),
};


namespace critical_chunks
{

static constexpr std::array ERRORS_HEADER_IMAGE =
{
    error_information::ErrorInformationChunks(ErrorChunkHeaderImage::IT_SHOULD_NOT_BE_INITIALIZED_AGAIN,
              "It shouldn't be initialized again"),

    error_information::ErrorInformationChunks(ErrorChunkHeaderImage::CAN_NOT_REDEFINE_WIDTH_IMAGE,
              "Can`t redefine width image!!!"),
    error_information::ErrorInformationChunks(ErrorChunkHeaderImage::CAN_NOT_REDEFINE_HEIGHT_IMAGE,
              "Can`t redefine height image!!!"),

    error_information::ErrorInformationChunks(ErrorChunkHeaderImage::THE_WIDTH_IMAGE_MUST_NOT_BE_LESS_THAN_ZERO,
            "The width image mustn't be less than zero!!!"),
    error_information::ErrorInformationChunks(ErrorChunkHeaderImage::THE_HEIGHT_IMAGE_MUST_NOT_BE_LESS_THAN_ZERO,
            "The height image mustn't be less than zero!!!"),

};

}

template<uint8_t N>
class Test_chunk
{
    enum class TypesErrorsChunks : uint8_t
    {
        NONE = 0,
        CHUNK,
        HEADER_IMAGE_CHUNK,
    };

    static consteval TypesErrorsChunks defineEnumTypesErrors(const uint8_t type)
    {
        switch (type)
        {
            case 1: return TypesErrorsChunks::CHUNK;
            case 2: return TypesErrorsChunks::HEADER_IMAGE_CHUNK;
            default: return TypesErrorsChunks::NONE;
        }
    }

    template<TypesErrorsChunks Type>
    static consteval auto defineArrayWithErrors()
    {
        if constexpr (Type == TypesErrorsChunks::CHUNK)
            return GENERAL_ERRORS_CHUNK;
        else if constexpr (Type == TypesErrorsChunks::HEADER_IMAGE_CHUNK)
            return critical_chunks::ERRORS_HEADER_IMAGE;
        else
            return std::array<TypesErrorsChunks, 1>();
    }

public:
    constexpr auto operator()() const
    {
        return defineArrayWithErrors<defineEnumTypesErrors(N)>();
    }
};

} // chunks

#endif //ERRORINFO_HPP
