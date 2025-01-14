#include "api.h"
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <mutex>
#include <string>
#include <utility>
#include <vector>
#include "crypto.h"

const int API::BUFFER_SIZE = 1024;
const uint8_t API::MAX_CONNECTIONS = 10;
//std::string API::port;
//std::mutex API::queue_mutex;
std::string API::port;
int API::server_socket;

//Struct for getaddrinfo
struct addrinfo API::hints;
struct addrinfo* API::servinfo;
struct addrinfo* API::p;

std::queue<std::string> API::message_queue;
std::mutex API::queue_mutex;

void API::SetPort(std::string port) {
    API::port = std::move(port);
}

bool API::Initialize() {
    message_queue = std::queue<std::string>();
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;

    // //Uses getaddrinfo instead of depreciated gethostbyname
    if (getaddrinfo(nullptr, port.c_str(), &hints, &servinfo) != 0) {
        std::cerr << "getaddrinfo error" << std::endl;
        return false;
    }

    std::cout << "Starting game server on port: " << port << "\n";

    // Create a socket
    //int server_socket;
    for (p = servinfo; p != nullptr; p = p->ai_next) {
        server_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (server_socket == -1) {
            continue;  // Try the next one
        }

        const int on = 1;
        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&on,
                       sizeof(on)) == -1) {
            std::cerr << "setsockopt failed" << std::endl;
            close(server_socket);
            continue;  // Try the next one
        }

        // Bind the socket
        if (bind(server_socket, p->ai_addr, p->ai_addrlen) == -1) {
            close(server_socket);
            continue;  // Try the next one
        }
        break;  // Successfully bound
    }

    //Exit if failed to bind
    if (p == nullptr) {
        std::cerr << "Failed to bind the socket" << "\n";
        //Free addrinfo structure as client_sd is connected
        freeaddrinfo(servinfo);
        return false;
    }

    //Free addrinfo structure as client_sd is connected
    freeaddrinfo(servinfo);
    std::cout << "Game server started successfully\n";
    // std::cout << server_socket_ << "\n";
    return true;
}

bool API::Start() {
    return Initialize();
}

bool API::Stop() {
    // Close the server socket
    close(server_socket);
    return true;
}

std::string API::GenerateKey(const std::string& key) {
    const std::string magic_string =
        key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    std::vector<unsigned char> hash = Crypto::sha1Hash(magic_string);
    return Crypto::base64Encode(hash);
}

void API::sendWebSocketMessage(int socket, const std::string& message) {
    std::vector<uint8_t> frame;
    // First byte: FIN (1) + Opcode (0x1 for text)
    frame.push_back(0x81);

    // Second byte: No masking (0) + payload length
    size_t message_length = message.size();
    if (message_length <= 125) {
        frame.push_back(static_cast<uint8_t>(message_length));
    } else if (message_length <= 65535) {
        frame.push_back(126);
        frame.push_back((message_length >> 8) & 0xFF);  // Most significant byte
        frame.push_back(message_length & 0xFF);  // Least significant byte
    } else {
        frame.push_back(127);
        for (int i = 7; i >= 0; --i) {
            frame.push_back((message_length >> (i * 8)) & 0xFF);
        }
    }

    // Add payload (message)
    frame.insert(frame.end(), message.begin(), message.end());

    // Send the frame
    send(socket, frame.data(), frame.size(), 0);
}

std::string API::decodeWebSocketFrame(const std::vector<uint8_t>& frame) {
    size_t index = 0;

    if (frame.size() < 2) {
        throw std::invalid_argument("Frame is too small to be valid.");
    }

    // First byte: FIN and Opcode
    uint8_t fin = frame[index] & 0x80;     // FIN bit
    uint8_t opcode = frame[index] & 0x0F;  // Opcode
    index++;

    // Second byte: Mask bit and Payload length
    bool masked = frame[index] & 0x80;           // Mask bit
    size_t payloadLength = frame[index] & 0x7F;  // Length (7 bits)
    index++;

    // Extended payload length
    if (payloadLength == 126) {
        payloadLength = (frame[index] << 8) | frame[index + 1];
        index += 2;
    } else if (payloadLength == 127) {
        payloadLength = 0;
        for (int i = 0; i < 8; ++i) {
            payloadLength = (payloadLength << 8) | frame[index + i];
        }
        index += 8;
    }
    // Masking key
    std::vector<uint8_t> maskingKey(4);
    if (masked) {
        maskingKey = {frame[index], frame[index + 1], frame[index + 2],
                      frame[index + 3]};
        index += 4;
    }

    // Payload data
    std::string payload;
    for (size_t i = 0; i < payloadLength; ++i) {
        uint8_t byte = frame[index + i];
        if (masked) {
            // Unmask payload byte
            byte ^= maskingKey[i % 4];
        }
        payload += static_cast<char>(byte);
    }

    return payload;
}

void API::PushMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    message_queue.push(message);
}

std::string API::GetNextMessage() {
    std::lock_guard<std::mutex> lock(queue_mutex);
    if (message_queue.empty()) {
        return "";
    }
    std::string return_message = message_queue.front();
    message_queue.pop();
    return return_message;
}

bool API::HasNextMessage() {
    return message_queue.empty();
}

int API::GetServerSocket() {
    return server_socket;
}

void API::Handshake(int client_socket) {
    //Buffer array
    std::array<char, BUFFER_SIZE> buffer = std::array<char, BUFFER_SIZE>();

    ssize_t request_bytes = read(client_socket, buffer.data(), BUFFER_SIZE - 1);
    if (request_bytes < 0) {
        std::cerr << "SERVER: Error reading request\n";
        //nullptr;
        return;
    }
    buffer[request_bytes] = '\0';
    std::string request(buffer.data());
    std::cout << "Request:\n" << request << "\n";
    // Locate the "Sec-WebSocket-Key" header
    size_t keyStart = request.find("Sec-WebSocket-Key: ");
    if (keyStart == std::string::npos) {
        std::cerr << "Sec-WebSocket-Key not found in request.\n";
        return;  //nullptr;
    }

    // Advance to the start of the key
    keyStart += std::string("Sec-WebSocket-Key: ").length();

    // Find the end of the key (newline or carriage return)
    size_t keyEnd = request.find("\r\n", keyStart);
    if (keyEnd == std::string::npos) {
        std::cerr << "Malformed request: Key line not terminated correctly.\n";
        return;  // nullptr;
    }

    // Extract the key
    std::string key = request.substr(keyStart, keyEnd - keyStart);
    std::cout << "Key:" << key << "<-end\n\n";

    std::string response = "HTTP/1.1 101 Switching Protocols\r\n";
    response += "Upgrade: websocket\r\n";
    response += "Connection: Upgrade\r\n";
    response += "Sec-WebSocket-Accept: " + API::GenerateKey(key);
    response += "\r\n\r\n";

    std::cout << response << "\n";
    write(client_socket, response.c_str(), response.size());
}