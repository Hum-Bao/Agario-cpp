#ifndef PELLET_H
#define PELLET_H

#include <cstdint>

class Pellet {
    public:
        const static uint8_t MIN_MASS = 1;
        const static uint8_t MAX_MASS = 5;
        explicit Pellet(const int& mass);
        Pellet();
        [[nodiscard]] int GetMass() const;

    private:
        uint8_t mass_;
};
#endif