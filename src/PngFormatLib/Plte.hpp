//
// Created by ganch on 04.12.2024.
//

#ifndef PLTE_HPP
#define PLTE_HPP
#include <cstdint>


namespace chunks::critical_chunks
{

class Plte {
private:
    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;
public:
    constexpr Plte()
        : _red(0), _green(0), _blue(0) {}
};

} // chunk

#endif //PLTE_HPP
