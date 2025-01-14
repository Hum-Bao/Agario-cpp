#ifndef PELLETMANAGER_H
#define PELLETMANAGER_H

#include <array>
#include <map>
#include "pellet.h"

class PelletManager {
    public:
        PelletManager();

        [[nodiscard]] std::map<std::pair<int, int>, Pellet> GetPellets() const;
        bool RemovePellet(std::pair<int, int> pellet);

    private:
        const static int MAX_PELLETS = 1000;
        const static int GRIDS = 4;
        std::map<std::pair<int, int>, Pellet> pellets_;
        // std::array<std::map<std::pair<int, int>, Pellet>, GRIDS> pellet_grid_;
};
#endif