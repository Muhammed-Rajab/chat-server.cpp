#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "server.h"
#include "rooms.h"
#include "logger.h"

#define PORT 8080

void handleClient(Room &room, int socket_fd)
{
    ClientInfo info = room.getInfo(socket_fd);

    while (true)
    {
        // DO CLIENT HANDLING SHIT
        // if receive message, broadcast it
        // if something wrong happens, do removal here.
        char buffer[1024] = {0};

        int bytes_received = sock::server::readFrom(socket_fd, buffer, 1024);

        if (bytes_received == 0)
        {
            // client has disconnected
            Logger::logf(Logger::INFO, "client disconnected from '%s': %s", room.name.c_str(), info.nickname.c_str());
            room.remove(socket_fd);
            close(socket_fd);
            break;
        }
        else if (bytes_received == -1)
        {
            Logger::log(Logger::ERROR, "error from reading client");
            Logger::logf(Logger::INFO, "client disconnected from '%s': %s", room.name.c_str(), info.nickname.c_str());
            room.remove(socket_fd);
            close(socket_fd);
            break;
        }
        else
        {
            std::ostringstream oss;
            oss << info.nickname << ": " << buffer;
            Logger::logf(Logger::MESSAGE, "%s@%s said: %s", info.nickname.c_str(), room.name.c_str(), buffer);

            room.broadcast(oss.str(), socket_fd);
        }
    }
}

int main()
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = sock::server::initialize();

    sock::server::bind(server_fd, address, PORT);

    sock::server::startListening(server_fd);

    Logger::logf(Logger::SUCCESS, "server started running at %d", PORT);

    Room hall{"hall"};

    while (true)
    {
        int newsocket_fd = sock::server::acceptConnection(server_fd, address);

        /*
        TODO: GET CONFIGURATION FROM USER
        intially, get nickname from the user
        save the nickname and socket stuff in a map or something?
        broadcast messages:
            - user join
            - user left
            - user send messages
        */

        // max nickname length 30
        char nickname[30] = {0};
        int NICKNAME_MAX_SIZE = 30;
        sock::server::readFrom(newsocket_fd, nickname, NICKNAME_MAX_SIZE);

        // create a new thread to listen to client
        // add client to room(now hall)
        ClientInfo info = hall.add(newsocket_fd, nickname);
        Logger::logf(Logger::INFO, "client joined '%s': %s", hall.name.c_str(), nickname);

        // CREATE A THREAD TO HANDLE CLIENT MESSAGES
        std::thread clientThread(handleClient, std::ref(hall), newsocket_fd);
        clientThread.detach();
    }

    close(server_fd);

    return EXIT_SUCCESS;
}