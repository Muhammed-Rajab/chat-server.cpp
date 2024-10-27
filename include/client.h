#ifndef CLIENT_4_CHAT_SERVER_H
#define CLIENT_4_CHAT_SERVER_H

#include "utils.h"
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace sock::client
{
    int initialize()
    {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        checkAndExit(fd < 0, "socket initialization failed\n");
        return fd;
    }

    sockaddr_in getSocketAddrIn(const int port, const char *address)
    {
        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        checkAndExit(inet_pton(AF_INET, address, &server_addr.sin_addr) <= 0, "invalid address\n");

        return server_addr;
    }

    void connectToServer(int fd, sockaddr_in &server_addr)
    {
        checkAndExit(connect(fd, (sockaddr *)(&server_addr), sizeof(server_addr)) < 0, "connection failed");
    }

    int sendTo(int fd, const char *message)
    {
        return send(fd, message, strlen(message), 0);
    }

    int readFrom(int fd, char *buffer, ssize_t buffer_size)
    {
        return read(fd, buffer, buffer_size);
    }

}

#endif