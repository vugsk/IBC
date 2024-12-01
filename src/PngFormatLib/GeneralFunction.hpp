//
// Created by ganch on 29.11.2024.
//

#ifndef GENERALFUNCTION_HPP
#define GENERALFUNCTION_HPP

#include <cstdint>
#include <type_traits>


namespace general_function
{

template<typename T, bool isUnsigned = true> requires(std::is_integral_v<T>)
class func_check_type_
{
public:
    constexpr auto operator()() const noexcept
    {
        if constexpr (sizeof(T) == 1)
        {
            if constexpr (isUnsigned)
                return uint8_t{1};
            else
                return int8_t{-1};
        }
        else if constexpr (sizeof(T) == 2)
        {
            if constexpr (isUnsigned)
                return uint16_t{1};
            else
                return int16_t{-1};
        }
        else if constexpr (sizeof(T) == 4)
        {
            if constexpr (isUnsigned)
                return uint32_t{1};
            else
                return int32_t{-1};
        }
        else
        {
            if constexpr (isUnsigned)
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

template<auto number> requires(std::is_integral_v<decltype(number)>)
inline constexpr auto func_defined_type_t = func_defined_type<number>();

template<auto start, auto end, auto step = 1, typename Function>
static constexpr void for_constexpr(Function&& func)
{
    if constexpr (step > 0 && start < end)
    {
        func(start);
        for_constexpr<start + step, end>(std::forward<Function>(func));
    }
}

}

#endif //GENERALFUNCTION_HPP
