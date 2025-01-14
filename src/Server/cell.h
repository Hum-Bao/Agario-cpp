#ifndef CELL_H
#define CELL_H

#include <array>
#include <cstdint>

//Can only consume cells 85% of size or smaller
class Cell {
    public:
        Cell();
        void SetMass(int mass);
        void SetRadius(float radius);

        [[nodiscard]] int GetMass() const;
        [[nodiscard]] float GetRadius() const;

    private:
        const static int MAX_MASS;
        const static uint8_t START_MASS;
        const static float START_RADIUS;
        int mass_;
        float radius_;
};
#endif