//
// Created by ganch on 08.12.2024.
//

#ifndef ERROROBJECT_HPP
#define ERROROBJECT_HPP

#include <stdexcept>
#include <utility>


namespace error_information
{

// can be using modification constexpr
template<typename Enumclass>
class ErrorInformationChunks
{
    Enumclass _error;
    const char* _text{};

public:
    constexpr          ErrorInformationChunks() noexcept = default;
    constexpr explicit ErrorInformationChunks(Enumclass&& err, const char* const&& text)
        : _error(std::forward<Enumclass>(err))
        , _text(std::forward<const char* const>(text)) {}
    constexpr ~ErrorInformationChunks() = default;
    constexpr ErrorInformationChunks(const ErrorInformationChunks& other) = default;
    constexpr ErrorInformationChunks(ErrorInformationChunks&& other) noexcept
        : _error(other._error), _text(other._text)
    {
        other._error = Enumclass::NONE;
        other._text  = nullptr;
    }

    constexpr ErrorInformationChunks& operator=(const ErrorInformationChunks& other)
    {
        if (this == &other)
            return *this;

        _error = other._error;
        _text  = other._text;

        return *this;
    }
    constexpr ErrorInformationChunks& operator=(ErrorInformationChunks&& other) noexcept
    {
        if (this == &other)
            return *this;

        _error = other._error;
        _text  = other._text;

        other._error = Enumclass::NONE;
        other._text  = nullptr;

        return *this;
    }

    [[nodiscard]] constexpr Enumclass error() const
    {
        return _error;
    }
    [[nodiscard]] constexpr const char* const& text() const
    {
        return _text;
    }
};

template<typename Enumclass, Enumclass Error, typename F,
         bool IsNotDefault = false, bool Value = true>
class check_errors_fn
{
    static constexpr auto ARRAY_E = F{}();

    template<auto Start = 0, auto End = ARRAY_E.size(), auto Step = 1>
    __forceinline static constexpr void iterationArrayWithError()
    {
        if constexpr (Step > 0 && Start < End)
        {
            if constexpr (ARRAY_E[Start].error() == Error)
                throw std::runtime_error(ARRAY_E[Start].text());
            iterationArrayWithError<Start + Step, End>();
        }
    }

public:
    constexpr void operator()(bool&& val = true) const
    {
        if constexpr (IsNotDefault && Value)
            iterationArrayWithError();
        else
            if (val)
                iterationArrayWithError();
    }
};

template<typename Enumclass, Enumclass Error, typename F,
         bool IsNotDefault = false, bool Value = true>
static inline constexpr check_errors_fn<Enumclass, Error, F,
                                        IsNotDefault, Value> checkError{};

}

#endif //ERROROBJECT_HPP
