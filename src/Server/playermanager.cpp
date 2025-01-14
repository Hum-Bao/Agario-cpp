#include "playermanager.h"
#include <iostream>
#include "api.h"
#include "player.h"

void PlayerManager::AddPlayer(const std::string& player_id, int socket) {
    std::lock_guard<std::mutex> lock(player_mutex_);
    players_.emplace(player_id, Player(player_id, socket));
}

Player* PlayerManager::GetPlayer(const std::string& player_id) {
    std::lock_guard<std::mutex> lock(player_mutex_);
    auto it = players_.find(player_id);
    if (it != players_.end()) {
        return &(it->second);
    }
    return nullptr;
}

void PlayerManager::RemovePlayer(const std::string& player_id) {
    std::lock_guard<std::mutex> lock(player_mutex_);
    players_.erase(player_id);
}

uint64_t PlayerManager::Size() const {
    return players_.size();
}

std::vector<std::string> PlayerManager::GetPlayerIDs() {
    std::vector<std::string> ids{};
    ids.reserve(players_.size());
    for (const auto& [id, player] : players_) {
        ids.push_back(id);
    }
    return ids;
}

void PlayerManager::PredictPlayerPositions() {
    std::lock_guard<std::mutex> lock(player_mutex_);
    for (auto& [id, player] : players_) {
        player.SetPosition();
        /*
        std::cout << "Player " << player.GetID()
                  << " Position: " << player.GetCoords().first << ","
                  << player.GetCoords().second << " | "
                  << player.GetVelocity().first << ","
                  << player.GetVelocity().second << "\n ";*/
    }
}

void PlayerManager::BroadcastMessage(const std::string& player_id,
                                     const std::string& message) {
    std::lock_guard<std::mutex> lock(player_mutex_);
    for (const auto& [id, player] : players_) {
        if (id == player_id) {
            continue;
        }
        API::sendWebSocketMessage(player.GetSocket(), message);
    }
}