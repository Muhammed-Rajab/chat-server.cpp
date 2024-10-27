#ifndef ROOMS_4_CHAT_SERVER_H
#define ROOMS_4_CHAT_SERVER_H

#include <string>
#include <mutex>
#include <thread>
#include <sys/socket.h>
#include <unordered_map>

#include "server.h"

struct ClientInfo
{
    int fd;
    std::string nickname;
};

class Room
{
private:
public:
    std::string name;
    std::mutex mutex_;
    std::unordered_map<int, ClientInfo> client_infos;

    Room(const std::string &name)
    {
        this->name = name;
    }

    void add(int fd, std::string nickname)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        client_infos[fd] = ClientInfo{fd, nickname};
        // TODO: broadcast to everyone
    }

    void remove(int fd)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = client_infos.find(fd);

        if (it != client_infos.end())
        {
            client_infos.erase(fd);
            // TODO: broadcast to everyone
        }
    }

    void broadcast(std::string message, int sender_fd)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        for (const std::pair<int, ClientInfo> &info : this->client_infos)
        {
            if (info.first != sender_fd)
            {
                send(info.second.fd, message.c_str(), message.size(), 0);
            }
        }
    }
};

#endif