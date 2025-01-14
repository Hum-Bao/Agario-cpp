#ifndef PLAYER_H
#define PLAYER_H

#include <shared_mutex>
#include <string>
#include <utility>
#include "cellgroup.h"

class Player {
    public:
        //Initialize player and generate random start coords
        Player();
        Player(std::string player_id, int socket);

        void SetCoords(std::pair<float, float> coords);
        void SetVelocity(std::pair<float, float> velocity);
        void SetViewport(std::pair<int, int> viewport);
        void SetPosition();
        void SetMass(int mass);
        void SetRadius(float radius);
        // void Spawn();

        [[nodiscard]] int GetSocket() const;
        [[nodiscard]] std::string GetID() const;

        [[nodiscard]] std::pair<float, float> GetCoords() const;
        [[nodiscard]] std::pair<float, float> GetVelocity() const;
        //Getviewport?
        [[nodiscard]] int GetGrid() const;
        [[nodiscard]] float GetRadius() const;
        [[nodiscard]] int GetMass() const;

        static std::pair<int, int> GenerateRandomCoords();

    private:
        // CellGroup player_cells_;
        Cell player_cell_;
        // std::pair<float, float> coords_;
        std::pair<float, float> coords_;
        std::pair<float, float> velocity_;
        std::pair<int, int> viewport_;
        int socket_;
        std::string id_;
        //  int grid_;
};
#endif