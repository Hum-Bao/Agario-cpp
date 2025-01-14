#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <sys/types.h>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include "player.h"

class PlayerManager {
    public:
        void AddPlayer(const std::string& player_id, int socket);
        Player* GetPlayer(const std::string& player_id);
        void RemovePlayer(const std::string& player_id);

        [[nodiscard]] uint64_t Size() const;

        //  void BroadcastLocation(const std::string& user_id);
        std::vector<std::string> GetPlayerIDs();

        void PredictPlayerPositions();
        //void UpdatePlayerPositions();

        void BroadcastMessage(const std::string& player_id,
                              const std::string& message);

    private:
        std::mutex player_mutex_;
        std::unordered_map<std::string, Player> players_;
};
#endif