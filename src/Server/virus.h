#ifndef VIRUS_H
#define VIRUS_H

#include <cstdint>
#include "cell.h"

class Virus {
    public:
        Virus();
        void force_split(Cell cell);

    private:
        const static uint8_t MIN_MASS = 100;
        const static uint8_t MAX_MASS = 178;
        //Virus starts with 100 mass, max mass is 178 (Base + 6 ejections).
        uint8_t mass_ = MIN_MASS;
        uint8_t ejections_;
};
#endif
