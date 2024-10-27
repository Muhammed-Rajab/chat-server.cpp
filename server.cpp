#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "server.h"
#include "rooms.h"

#define PORT 8080

int main()
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = sock::server::initialize();

    sock::server::bind(server_fd, address, PORT);

    sock::server::startListening(server_fd);

    std::cout << "server listening on port " << PORT << "\n";

    Room hall{"hall"};

    while (true)
    {
        int newsocket_fd = sock::server::acceptConnection(server_fd, address);
        std::cout << "new connection!\n";

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
        hall.add(newsocket_fd, nickname);
    }

    close(server_fd);

    return EXIT_SUCCESS;
}