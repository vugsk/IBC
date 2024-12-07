//
// Created by ganch on 29.11.2024.
//

#ifndef GENERALFUNCTION_HPP
#define GENERALFUNCTION_HPP

#include <cstdint>
#include <type_traits>


namespace general_function
{

template<typename T, bool IsUnsigned = true> requires(std::is_integral_v<T>)
class func_check_type_
{
public:
    constexpr auto operator()() const noexcept
    {
        if constexpr (sizeof(T) == 1)
        {
            if constexpr (IsUnsigned)
                return uint8_t{1};
            else
                return int8_t{-1};
        }
        else if constexpr (sizeof(T) == 2)
        {
            if constexpr (IsUnsigned)
                return uint16_t{1};
            else
                return int16_t{-1};
        }
        else if constexpr (sizeof(T) == 4)
        {
            if constexpr (IsUnsigned)
                return uint32_t{1};
            else
                return int32_t{-1};
        }
        else
        {
            if constexpr (IsUnsigned)
                return uint64_t{1};
            else
                return int64_t{-1};
        }
    }
};

template<typename T, bool isUnsigned = true>
static inline constexpr func_check_type_<T, isUnsigned> testT{};


template<typename T> requires(std::is_integral_v<T>)
class func_inversion_type_int_
{
public:
    constexpr auto operator()() const noexcept
    {
        if constexpr (std::is_signed_v<T>)
            return testT<T>();
        else
            return testT<T, false>();
    }
};

template<typename T>
static inline constexpr func_inversion_type_int_<T> testInversion{};


template<auto number> requires(std::is_integral_v<decltype(number)>)
struct func_defined_type
{
    constexpr auto operator()() const noexcept
    {
        if constexpr (number < INT8_MAX && number > INT8_MIN)
            return static_cast<int8_t>(number);
        else if constexpr (number < UINT8_MAX && number > 0)
            return static_cast<uint8_t>(number);
        else if constexpr (number < INT16_MAX && number > INT16_MIN)
            return static_cast<int16_t>(number);
        else if constexpr (number < UINT16_MAX && number > 0)
            return static_cast<uint16_t>(number);
        else if constexpr (number < INT32_MAX && number > INT32_MIN)
            return static_cast<int32_t>(number);
        else if constexpr (number < UINT32_MAX && number > 0)
            return static_cast<uint32_t>(number);
        else if constexpr (number < INT64_MAX && number > INT64_MIN)
            return static_cast<int64_t>(number);
        else if constexpr (number < UINT64_MAX && number > 0)
            return static_cast<uint64_t>(number);
    };
};

template<auto Number> requires(std::is_integral_v<decltype(Number)>)
inline constexpr auto func_defined_type_t = func_defined_type<Number>();

template<auto Start, auto End, auto Step = 1, typename Function>
static constexpr void ForConstexpr(Function&& func) noexcept
{
    if constexpr (Step > 0 && Start < End)
    {
        func(Start);
        for_constexpr<Start + Step, End>(std::forward<Function>(func));
    }
}

template<typename Enumclass, Enumclass Error, typename F>
class func_substing
{
public:
    constexpr const char* operator()(const uint32_t begin, const uint32_t end,
        const char* data) const
    {
        error_information::checkError<Enumclass, Error, F>(begin > end ||
                                                              begin == end);
        const auto sub = new char[end - begin + 1];
        for (uint32_t i = begin; i < end; i++)
            sub[i - begin] = data[i];
        sub[end - begin] = '\0';
        return sub;
    }
};

template<typename Enumclass, Enumclass Error, typename F>
static inline constexpr func_substing<Enumclass, Error, F> SubString{};

}

#endif //GENERALFUNCTION_HPP
