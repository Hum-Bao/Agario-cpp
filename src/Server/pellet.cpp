#include "pellet.h"

const static uint8_t MIN_MASS = 1;
const static uint8_t MAX_MASS = 5;

Pellet::Pellet(const int& mass) : mass_(mass) {}

Pellet::Pellet() : mass_(1) {}

int Pellet::GetMass() const {
    return mass_;
}