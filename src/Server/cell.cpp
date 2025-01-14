#include "cell.h"
#include <cmath>

const int Cell::MAX_MASS = 22500;
const uint8_t Cell::START_MASS = 10;
const float Cell::START_RADIUS = 31.6227766017;

Cell::Cell() : mass_(Cell::START_MASS), radius_(START_RADIUS) {}

void Cell::SetMass(int mass) {
    mass_ = mass;
}

void Cell::SetRadius(float radius) {
    radius_ = radius;
}

int Cell::GetMass() const {
    return mass_;
}

float Cell::GetRadius() const {
    return radius_;
}