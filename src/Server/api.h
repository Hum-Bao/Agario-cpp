#ifndef API_H
#define API_H

#include <bits/types/struct_timeval.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <array>
#include <cstring>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>

class API {
    public:
        static void SetPort(std::string port);

        static std::string GenerateKey(const std::string& key);
        // explicit API(std::string port);

        // Initial connection via TCP, then game updates via UDP?
        static void* Listen(void* arg);

        static bool Start();

        static bool Stop();
        static bool Connect();
        static bool Disconnect();
        static bool HasNextMessage();
        static std::string GetNextMessage();
        static void PushMessage(const std::string& message);

        static std::string decodeWebSocketFrame(
            const std::vector<uint8_t>& frame);
        static void sendWebSocketMessage(int socket,
                                         const std::string& message);

        static int GetServerSocket();

        static void Handshake(int client_socket);

        // void SendToPlayer(const int& socket, const std::string& message);

    private:
        const static int BUFFER_SIZE;
        const static uint8_t MAX_CONNECTIONS;

        static std::string port;

        static int server_socket;

        //Struct for getaddrinfo
        static struct addrinfo hints;
        static struct addrinfo* servinfo;
        static struct addrinfo* p;

        static bool Initialize();
        static std::queue<std::string> message_queue;
        static std::mutex queue_mutex;

        // void* create_client_thread(void* client_socket_ptr);
};

#endif