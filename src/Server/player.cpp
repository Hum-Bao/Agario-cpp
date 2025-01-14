#include "player.h"
#include <random>
#include <utility>
#include "map.h"

Player::Player() : socket_(-1) {}

Player::Player(std::string player_id, int socket)
    : id_(std::move(player_id)), socket_(socket) {}

void Player::SetRadius(float radius) {
    player_cell_.SetRadius(radius);
}

float Player::GetRadius() const {
    // return player_cells_.GetCell(0).GetRadius();
    return player_cell_.GetRadius();
}

void Player::SetMass(int mass) {
    player_cell_.SetMass(mass);
}

int Player::GetMass() const {
    // return player_cells_.GetCell(0).GetMass();
    return player_cell_.GetMass();
}

void Player::SetViewport(std::pair<int, int> viewport) {
    viewport_ = viewport;
}

void Player::SetCoords(std::pair<float, float> coords) {
    coords_ = coords;
    /*
    if (coords.first < Map::MAP_WIDTH / 2) {
        if (coords.second > Map::MAP_HEIGHT / 2) {
            grid_ = 1;
        } else {
            grid_ = 4;
        }
    } else {
        if (coords.second > Map::MAP_HEIGHT / 2) {
            grid_ = 2;
        } else {
            grid_ = 3;
        }
    }
    */
}

void Player::SetVelocity(std::pair<float, float> velocity) {
    velocity_ = velocity;
}

void Player::SetPosition() {
    coords_.first += velocity_.first;
    coords_.second += velocity_.second;

    if (coords_.first < 0) {
        coords_.first = 0;
    }
    if (coords_.first > Map::MAP_WIDTH) {
        coords_.first = Map::MAP_WIDTH;
    }

    if (coords_.second < 0) {
        coords_.second = 0;
    }

    if (coords_.second > Map::MAP_HEIGHT) {
        coords_.second = Map::MAP_HEIGHT;
    }
}

std::pair<int, int> Player::GenerateRandomCoords() {
    std::random_device rand;
    std::mt19937 gen(rand());
    std::uniform_int_distribution<> random_int(0, Map::MAP_HEIGHT);
    return {random_int(gen), random_int(gen)};
}

int Player::GetSocket() const {
    return socket_;
}

std::string Player::GetID() const {
    return id_;
}

std::pair<float, float> Player::GetCoords() const {
    return coords_;
}

/*
int Player::GetGrid() const {
    return grid_;
}*/

std::pair<float, float> Player::GetVelocity() const {
    return velocity_;
}