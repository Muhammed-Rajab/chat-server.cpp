#ifndef ROOMS_4_CHAT_SERVER_H
#define ROOMS_4_CHAT_SERVER_H

#include <string>
#include <sstream>
#include <mutex>
#include <thread>
#include <sys/socket.h>
#include <unordered_map>

#include "server.h"
#include "events.h"

using json = nlohmann::json;

struct ClientInfo
{
    int fd;
    std::string nickname;
};

class Room
{
private:
    std::mutex mutex_;

public:
    std::string name;
    std::unordered_map<int, ClientInfo> client_infos;

    Room(const std::string &name)
    {
        this->name = name;
    }

    ClientInfo add(int fd, std::string nickname)
    {
        std::ostringstream oss;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            client_infos[fd] = ClientInfo{fd, nickname};
            oss << nickname << " has joined\n";
        }

        this->broadcast({"joined",
                         json{
                             {"nickname", nickname},
                         }
                             .dump(2)},
                        fd);

        return client_infos[fd];
    }

    void remove(int fd)
    {
        std::ostringstream oss;
        ClientInfo info;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = client_infos.find(fd);

            if (it != client_infos.end())
            {
                info = it->second;
                client_infos.erase(fd);

                oss << info.nickname << " has left\n";
            }
        }
        this->broadcast({"left",
                         json{
                             {"nickname", info.nickname},
                         }
                             .dump(2)},
                        fd);
    }

    void broadcast(Event event, int sender_fd)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        for (const std::pair<int, ClientInfo> &info : this->client_infos)
        {
            if (info.first != sender_fd)
            {
                std::string data = event.toJson().dump(2);
                send(info.second.fd, data.c_str(), data.size(), 0);
            }
        }
    }

    ClientInfo getInfo(int fd)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = this->client_infos.at(fd);
        return it;
    }
};

#endif