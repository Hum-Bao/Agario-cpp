#include <pthread.h>
#include <sys/socket.h>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include "api.h"
#include "pellet.h"
#include "pelletmanager.h"
#include "player.h"
#include "playermanager.h"

//Lose 0.2% of mass per second
//double loss_rate = 0.2;
//static API api("54321");
//static API api;

//server size is from -7071:-7071 to 7071:7071 or 14142x14142 units

/*
  mass = size * size / 100

   baseResolution = new Vector2(1920, 1080);
   scaleBasis = power(min(64 / totalSize, 1), 0.4);
   ratio = max(displaySize.X / baseResolution.X, displaySize.Y /
                                                           baseResolution.Y);
   scale = scaleBasis * ratio;

   filteredScale = (9 * filteredScale + scale) / 10;

   viewportSize = displaySize / filteredScale

   baseResolution = new Vector2(1920, 1080);
   viewportSize = baseResolution / filteredScale;

   center = average(ownedCells.Position);

   viewRadius = 1024 / Math.pow(Math.min(64.0 / totalCellRadius, 1), 0.4);

   TTR = max(30, rounddown(cell.Size * 0.2)); // current merge time
   30 + Combined_Mass * 0.2 // old merge time

   speed = 2.2 * size^(-0.439)

*/

//Atomic bool for allowing server to gracefully close on CTRL + C
std::atomic<bool> server_running(true);

//Signal handler to catch SIGINT (CTRL + C)
void handle_shutdown_signal(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout
            << "GAME SERVER: Shutdown signal received. Stopping server...\n\n";
        server_running = false;
    }
}

const int TICKRATE = 60;
constexpr uint64_t TICK_DURATION_US = 1000000 / TICKRATE;

const static int BUFFER_SIZE = 1024;

PlayerManager player_manager;
PelletManager pellet_manager;

void DisconnectPlayer(const std::string& player_id) {
    int socket = player_manager.GetPlayer(player_id)->GetSocket();
    API::sendWebSocketMessage(socket, "GAMEOVER");
    player_manager.RemovePlayer(player_id);
    close(socket);
}

void* HandleClient(void* client_socket_ptr) {
    std::cout << "NEW CONNECTION\n";
    const int socket = *static_cast<int*>(client_socket_ptr);
    delete static_cast<int*>(client_socket_ptr);  // Clean up memory

    // Example: Assign a unique player ID (can be based on socket or something else)
    const std::string player_id = "player_" + std::to_string(socket);
    player_manager.AddPlayer(player_id, socket);  // Add player to the manager
    Player* current_player = player_manager.GetPlayer(player_id);

    std::array<char, BUFFER_SIZE> buffer = std::array<char, BUFFER_SIZE>();

    API::Handshake(socket);

    bool sent_pellets = false;

    // ssize_t request_bytes = 0;

    std::string message;

    // Handshake and communication loop
    while (server_running) {
        ssize_t request_bytes = read(socket, buffer.data(), BUFFER_SIZE);
        if (request_bytes <= 0) {
            std::cerr << "Error reading frame\n";
            break;
        }

        std::vector<uint8_t> frame(buffer.begin(),
                                   buffer.begin() + request_bytes);

        message = API::decodeWebSocketFrame(frame);

        // int last_grid = 0;

        if (message.find("SPAWN") != std::string::npos) {
            std::pair<int, int> random_coords = Player::GenerateRandomCoords();
            current_player->SetCoords(random_coords);
            std::string spawn_message = "SPAWNED:";
            spawn_message += std::to_string(random_coords.first);
            spawn_message += ":" + std::to_string(random_coords.second);
            spawn_message += ":" + std::to_string(current_player->GetMass());
            spawn_message += ":" + std::to_string(current_player->GetRadius());
            std::cout << spawn_message << "\n";

            API::sendWebSocketMessage(socket, spawn_message);
        } else if (message.find("CONSUMED:") != std::string::npos) {

            uint8_t start = message.find(':');
            uint8_t end = message.find(',');
            std::pair<int, int> eaten_pellet = {
                stoi(message.substr(start + 1, end - start)),
                stoi(message.substr(end + 1))};

            std::string response;

            if (pellet_manager.RemovePellet(eaten_pellet)) {
                response = "CONSUMED:";
                response += std::to_string(eaten_pellet.first) + ":";
                response += std::to_string(eaten_pellet.second);
            }
            // std::cout << response << "\n";

            player_manager.BroadcastMessage(player_id, response);

        } else if (message.find("LOCATION:") != std::string::npos) {
            // std::cout << message << "\n";
            uint8_t start = message.find(':');
            uint8_t end = message.find(',');
            current_player->SetCoords(
                {std::stod(message.substr(start + 1, end - start)),
                 std::stod(message.substr(end + 1))});
            /*
            else if (message.find("AT:") != std::string::npos) {
                std::pair<int, int> current_coords;
                try {
                    current_coords = {
                        stoi(message.substr(3, message.find(',') - 3)),
                        stoi(message.substr(message.find(',') + 1))};
                } catch (std::exception e) {
                    std::cerr << "Location update error: " << e.what() << "\n";
                    std::cerr << message.substr(3, message.find(',') - 3)
                              << "\n";
                    std::cerr << message.substr(message.find(',') + 1) << "\n";
                    player_manager.RemovePlayer(player_id);
                    close(socket);
                    return nullptr;
                }
                current_player->SetCoords(current_coords);
                */
            /*
            std::cout << message << "\n";
            uint8_t start = message.find(':');
            uint8_t end = message.find(',');
            float x = std::stod(message.substr(start + 1, end - start));
            float y = std::stod(message.substr(end + 1));

            if (x < current_player->GetCoords().first ||
                y != current_player->GetCoords().second) {

                std::string response = "DESYNC:";
                response +=
                    (std::to_string(current_player->GetCoords().first) + ":");
                response += std::to_string(current_player->GetCoords().second);
                API::sendWebSocketMessage(socket, response);
                std::cout << response << "\n";
            }
            */
        } /* else if (message.find("VELOCITY:") != std::string::npos) {
            uint8_t start = message.find(':');
            uint8_t end = message.find(',');

            // std::cout << message.substr(9, index - 9) << ","
            //   << message.substr(index + 1, index) << "\n";
            current_player->SetVelocity(
                {std::stod(message.substr(start + 1, end - start)),
                 std::stod(message.substr(end + 1))});
        } */

        else if (message.find("SIZE:") != std::string::npos) {
            // std::cout << message << "\n";
            uint8_t start = message.find(':');
            uint8_t end = message.find(',');
            current_player->SetMass(
                std::stoi(message.substr(start + 1, end - start)));
            current_player->SetRadius(std::stod(message.substr(end + 1)));

        } else if (message.find("KILLED:") != std::string::npos) {
            uint8_t start = message.find(':');
            std::string enemy_id = message.substr(start + 1);
            if (enemy_id == "self") {
                std::cout << "Player: " << player_id << " left\n";
                break;
            }
            DisconnectPlayer(enemy_id);
        } else {
            std::cout << message << "\n";
        }

        if (player_manager.Size() > 1) {
            std::pair<int, int> current_user_coords =
                current_player->GetCoords();

            std::string message = "ENEMY:";
            message += (player_id + ":");
            message += (std::to_string(current_user_coords.first) + ":");
            message += (std::to_string(current_user_coords.second) + ":");
            message += (std::to_string(current_player->GetMass()) + ":");
            message += std::to_string(current_player->GetRadius());

            //std::cout << message << "\n";

            player_manager.BroadcastMessage(player_id, message);
        }

        if (!sent_pellets) {
            std::map<std::pair<int, int>, Pellet> pellets =
                pellet_manager.GetPellets();
            for (std::pair<std::pair<int, int>, Pellet> pellet : pellets) {
                message = "PELLET:";
                message += std::to_string(pellet.first.first) + ":";
                message += std::to_string(pellet.first.second) + ":";
                message += std::to_string(pellet.second.GetMass());
                API::sendWebSocketMessage(socket, message);
            }
            sent_pellets = true;
        }
    }

    // Remove the player when done
    player_manager.RemovePlayer(player_id);
    close(socket);
    return nullptr;
}

void* ListenForConnections(void* server_socket_ptr) {
    const int server_socket = *static_cast<int*>(server_socket_ptr);
    delete static_cast<int*>(server_socket_ptr);  // Clean up memory

    std::cout << "GAME SERVER: Listening for connections\n";
    // Start listening
    if (listen(server_socket, 10) == -1) {
        std::cerr << "Server listen error" << "\n";
        close(server_socket);
        return nullptr;
    }

    while (server_running) {
        sockaddr new_sock_addr{};
        socklen_t new_sock_addr_size = sizeof(new_sock_addr);

        int* client_socket = new int;

        // int client_socket = accept(server_socket, nullptr, nullptr);
        *client_socket =
            accept(server_socket, &new_sock_addr, &new_sock_addr_size);

        if (*client_socket == -1) {
            std::cerr << "Accept error" << "\n";
            delete client_socket;  // Clean up memory if accept fails
            return nullptr;
            //continue;
        }

        // Create a new thread for each new connection
        pthread_t client_thread = 0;
        if (pthread_create(&client_thread, nullptr, HandleClient,
                           client_socket) > 0) {
            std::cerr << "Thread not created. \n";
            close(*client_socket);
            // Clean up memory if thread creation fails
            delete client_socket;
        }
        pthread_detach(client_thread);
    }

    return nullptr;
}

void Listen() {
    pthread_t listen_thread = 0;
    int* server_socket = new int(API::GetServerSocket());
    if (pthread_create(&listen_thread, nullptr, ListenForConnections,
                       server_socket) > 0) {
        std::cerr << "Thread not created. \n";
        delete server_socket;
    }
    pthread_detach(listen_thread);
}

void GameLoop() {
    while (server_running) {
        auto current_tick = std::chrono::high_resolution_clock::now();
        player_manager.PredictPlayerPositions();

        auto tick_end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            tick_end - current_tick);

        // std::cout << "Tick duration: " << duration.count() << " ms\n";

        if (duration.count() < TICK_DURATION_US) {
            std::this_thread::sleep_for(
                std::chrono::microseconds(TICK_DURATION_US - duration.count()));
        }
    }
}

int main(int argc, char* argv[]) {
    std::string port = "54321";
    if (argc == 2) {
        port = argv[1];
    }

    //Signal handler for SIGINT
    struct sigaction sigint_handler {};

    sigint_handler.sa_handler = handle_shutdown_signal;
    sigemptyset(&sigint_handler.sa_mask);
    sigint_handler.sa_flags = 0;
    sigaction(SIGINT, &sigint_handler, nullptr);
    sigaction(SIGTERM, &sigint_handler, nullptr);

    API::SetPort(port);
    API::Start();
    Listen();
    pellet_manager = PelletManager();

    GameLoop();
}