#include "pelletmanager.h"
#include <iostream>
#include <random>
#include "map.h"
#include "pellet.h"
#include "player.h"

PelletManager::PelletManager() {
    std::random_device rand;
    std::mt19937 gen(rand());
    std::uniform_int_distribution<> random_mass(Pellet::MIN_MASS,
                                                Pellet::MAX_MASS);
    for (int count = 0; count < PelletManager::MAX_PELLETS; count++) {
        pellets_[Player::GenerateRandomCoords()] = Pellet(random_mass(gen));
    }
    // std::uniform_int_distribution<> grid_x;
    //std::uniform_int_distribution<> grid_y;
    /*
    pellet_grid_ = std::array<std::map<std::pair<int, int>, Pellet>, GRIDS>();
    for (int map_num = 0; map_num < PelletManager::GRIDS; map_num++) {
        pellet_grid_[map_num] = std::map<std::pair<int, int>, Pellet>();
        switch (map_num) {
            case 0: {
                grid_x = std::uniform_int_distribution<>(0, Map::MAP_WIDTH / 2);

                grid_y = std::uniform_int_distribution<>(Map::MAP_HEIGHT / 2,
                                                         Map::MAP_HEIGHT);
                break;
            }
            case 1: {
                grid_x = std::uniform_int_distribution<>(Map::MAP_WIDTH / 2,
                                                         Map::MAP_WIDTH);

                grid_y = std::uniform_int_distribution<>(Map::MAP_HEIGHT / 2,
                                                         Map::MAP_HEIGHT);
                break;
            }
            case 2: {
                grid_x = std::uniform_int_distribution<>(Map::MAP_WIDTH / 2,
                                                         Map::MAP_WIDTH);

                grid_y =
                    std::uniform_int_distribution<>(0, Map::MAP_HEIGHT / 2);
                break;
            }
            case 3: {
                grid_x = std::uniform_int_distribution<>(0, Map::MAP_WIDTH / 2);
                grid_y =
                    std::uniform_int_distribution<>(0, Map::MAP_HEIGHT / 2);
                break;
            }
        }
        for (int count = 0;
             count < PelletManager::MAX_PELLETS / PelletManager::GRIDS;
             count++) {
            std::pair<int, int> loc = {grid_x(gen), grid_y(gen)};
            std::pair<std::pair<int, int>, Pellet> pel = {
                loc, Pellet(random_mass(gen))};
            pellet_grid_[map_num].insert(pel);
        }
    }
    */
}

/*
std::map<std::pair<int, int>, Pellet> PelletManager::GetPellets(
    const int& index) const {
    return pellet_grid_[index];
}*/

bool PelletManager::RemovePellet(std::pair<int, int> pellet) {
    auto it = pellets_.find(pellet);
    if (it != pellets_.end()) {
        pellets_.erase(it);
        std::cout << "Pair with key " << pellet.first << "," << pellet.second
                  << " removed.\n";
        return true;
    } else {
        std::cout << "Pair with key " << pellet.first << "," << pellet.second
                  << " not found.\n";
        return false;
    }
}

std::map<std::pair<int, int>, Pellet> PelletManager::GetPellets() const {
    return pellets_;
}