#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"

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

    while (true)
    {
        int newsocket_fd = sock::server::acceptConnection(server_fd, address);

        char buffer[1024] = {0};
        read(newsocket_fd, buffer, 1024);
        std::cout << "client: " << buffer;

        const char *message = "send me your location\n";
        send(newsocket_fd, message, strlen(message), 0);

        close(newsocket_fd);
    }

    close(server_fd);

    return EXIT_SUCCESS;
}