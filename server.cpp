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
        int BUFFER_SIZE = 1024;
        sock::server::readFrom(newsocket_fd, buffer, BUFFER_SIZE);

        // std::cout << "client: " << buffer;

        const char *message = "send me your location\n";
        sock::server::sendTo(newsocket_fd, message);

        close(newsocket_fd);
    }

    close(server_fd);

    return EXIT_SUCCESS;
}