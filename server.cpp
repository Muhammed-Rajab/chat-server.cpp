#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

void checkAndExit(bool condition, char *message, int errorCode = EXIT_FAILURE)
{
    if (condition)
    {
        std::cerr << message;
        exit(errorCode);
    }
}

namespace sock::core
{
    int initializeSocket()
    {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        checkAndExit(fd <= 0, "socket intialization failed");

        int opt = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        return fd;
    }

    void bindSocket(const int fd, sockaddr_in &address, const int port)
    {
        address.sin_port = htons(port);
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;

        checkAndExit(bind(fd, (struct sockaddr *)(&address), sizeof(address)) < 0, "bind failed\n");
    }

    void startListening(const int fd)
    {
        checkAndExit(listen(fd, 3) < 0, "listen failed\n");
    }

    int acceptConnection(const int fd, sockaddr_in &address)
    {
        int addrlen = sizeof(address);
        int new_fd = accept(fd, (sockaddr *)(&address), (socklen_t *)(&addrlen));
        checkAndExit(new_fd < 0, "accept failed\n");

        return new_fd;
    }
}

int main()
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = sock::core::initializeSocket();

    sock::core::bindSocket(server_fd, address, PORT);

    sock::core::startListening(server_fd);

    std::cout << "server listening on port " << PORT << "\n";

    while (true)
    {
        int newsocket_fd = sock::core::acceptConnection(server_fd, address);

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