#ifndef CELLGROUP_H
#define CELLGROUP_H

#include <array>
#include <cstdint>
#include "cell.h"

class CellGroup {
    public:
        CellGroup();
        //Must wait 30 sec + 2% of total mass seconds before cells can merge. Min 35 mass to merge
        void Merge();
        //Eject pellet from all cells above 35 mass. Must be 21 mass+ to consume. Ejection removes 18 mass from user. Consuming ejection gives 14 mass;
        void Split();

        [[nodiscard]] Cell GetCell(int index) const;

    private:
        const static uint8_t MAX_SIZE = 16;
        const static uint8_t MIN_SIZE = 1;
        uint8_t active_cells_;
        std::array<Cell, MAX_SIZE> cell_group_;
};
#endif