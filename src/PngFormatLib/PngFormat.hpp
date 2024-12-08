//
// Created by ganch on 25.11.2024.
//

#ifndef PNGFORMAT_HPP
#define PNGFORMAT_HPP

#include <cstdint>
#include <fstream>

#include "DataImage.hpp"
#include "EndImage.hpp"
#include "ErrorObject.hpp"
#include "GeneralFunction.hpp"
#include "HeaderImage.hpp"
#include "Signature.hpp"

// struct DopChunk
// {
// private:
//     struct Gama final : Chunk
//     {
//         uint32_t imageGamma{};
//     };
//     struct Srgb final : Chunk
//     {
//         uint8_t renderingIntent{};
//     };
//     struct Phys final : Chunk
//     {
//         uint32_t xAxis{};
//         uint32_t yAxis{};
//         uint8_t unitSpecifier{};
//     };
//
// public:
//     Gama gama;
//     Srgb srgb;
//     Phys phys;
// };

namespace png_formation
{
enum class ErrorPngFormat : uint8_t
{
    NONE = 0,

    CAN_NOT_BE_OPEN_FILE,
    CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING_IN_PNG_FORMAT
};

static constexpr std::array ERROR_PNG_FORMAT =
{
    error_information::ErrorInformationChunks(
        ErrorPngFormat::CAN_NOT_BE_OPEN_FILE,
        "Can't be open file!!!"),
    error_information::ErrorInformationChunks(
        ErrorPngFormat::CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING_IN_PNG_FORMAT,
        "Can't was crash string on substring in png format!!!"),
};

enum class ModeFile : uint8_t
{
    NONE = 0,
    READ,
    WRITE,
    MIXED,
};

template<ModeFile Mode>
class PngFormat
{
    static constexpr auto TE = []{ return ERROR_PNG_FORMAT; };

    chunks::critical_chunks::HeaderImage* _headerImage;
    chunk::critical_chunk::DataImage _dataImage;
    chunks::critical_chunks::EndImage* _endImage;

    signature::Signature* _signature;
    const char* _fileName;
    const char* _content;
    uint32_t _size{};

    template<ErrorPngFormat Error>
    static constexpr void checkErrors(bool&& val)
    {
        if (std::forward<bool>(val))
            error_information::checkError<ErrorPngFormat, Error, decltype(TE),
                true, true>();
    }

    // doing, what build on step compilation
    [[nodiscard]] constexpr uint32_t defineSize() const
    {
        std::ifstream file(_fileName, std::ios::in | std::ios::binary);
        checkErrors<ErrorPngFormat::CAN_NOT_BE_OPEN_FILE>(!file.is_open());
        file.seekg(0, std::ios::end);
        const std::streampos end = file.tellg();
        file.seekg(0, std::ios::beg);
        file.close();
        return static_cast<uint32_t>(end) + 1;
    }

    // doing, what build on step compilation
    [[nodiscard]] constexpr const char* readFormFileContent() const
    {
        // check error -> mode WRITE and NONE
        std::ifstream file(_fileName, std::ios::in | std::ios::binary);
        checkErrors<ErrorPngFormat::CAN_NOT_BE_OPEN_FILE>(!file.is_open());
        const auto content = new char[_size + 1];
        file.read(content, _size);
        content[_size] = '\0';
        file.close();
        return content;
    }

    [[nodiscard]] __forceinline static constexpr const char* subString(
        const uint32_t begin, const uint32_t end, const char* const& data)
    {
        return general_function::SubString<ErrorPngFormat,
            ErrorPngFormat::CAN_NOT_WAS_CRASH_STRING_ON_SUBSTRING_IN_PNG_FORMAT,
            decltype(TE)>(begin, end, data);
    }

public:
    constexpr explicit PngFormat(const char* filename)
        : _headerImage(nullptr)
        , _endImage(nullptr)
        , _signature(nullptr)
        , _fileName(filename)
        , _content(nullptr)
    {
        if constexpr (Mode == ModeFile::READ)
        {
            _size = defineSize();
            _content = readFormFileContent();
            _signature = signature::Signature::create(0, 8,
                subString(0, 8, _content));
        }
        else if constexpr (Mode == ModeFile::WRITE)
        {

        }
        else if constexpr (Mode == ModeFile::MIXED)
        {

        }
        else
        {

        }
    }
    constexpr ~PngFormat()
    {
        if (!_signature)
            return;
        if (!_headerImage)
            return;
        if (!_endImage)
            return;

        delete _signature;
        delete _headerImage;
        delete _endImage;
    }

    [[nodiscard]] constexpr const signature::Signature* signature1() const
    {
        return _signature;
    }
    [[nodiscard]] constexpr const char* fileName() const
    {
        return _fileName;
    }
    [[nodiscard]] constexpr uint32_t size() const
    {
        return _size;
    }
};

}





#endif //PNGFORMAT_HPP
