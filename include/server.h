#ifndef SERVER_4_CHAT_SERVER_H
#define SERVER_4_CHAT_SERVER_H

#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "utils.h"

namespace sock::server
{
    int initialize()
    {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        checkAndExit(fd < 0, "socket intialization failed");

        int opt = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        return fd;
    }

    void bind(const int fd, sockaddr_in &address, const int port)
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

    void sendMessage(int fd, const char *message)
    {
        send(fd, message, strlen(message), 0);
    }

    void receiveMessage(int fd, char *buffer, ssize_t buffer_size)
    {
        read(fd, buffer, buffer_size);
    }
}

#endif