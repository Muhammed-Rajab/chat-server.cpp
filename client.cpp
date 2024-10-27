#include <iostream>
#include <thread>
#include "client.h"

#define PORT 8080

void receiveMessages(int socket_fd)
{
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE] = {0};

    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);

        int bytesRead = sock::client::readFrom(socket_fd, buffer, BUFFER_SIZE - 1);
        if (bytesRead <= 0)
        {
            std::cout << "Server disconnected or error occurred.\n";
            break;
        }
        std::cout << buffer << std::endl;
    }
}

void sendMessages(int socket_fd)
{
    std::string message;
    while (true)
    {
        std::getline(std::cin, message);
        if (sock::client::sendTo(socket_fd, message) == -1)
        {
            std::cerr << "Error sending message.\n";
            break;
        }
    }
}

int main(int argc, char **argv)
{
    // * GET NICKNAME FROM USER
    if (argc == 1)
    {
        std::cerr << "provide a username\n";
        return EXIT_FAILURE;
    }
    std::string nickname{argv[1]};

    // * SOCK INIT
    int sock = sock::client::initialize();
    sockaddr_in server_addr = sock::client::getSocketAddrIn(PORT, "127.0.0.1");
    sock::client::connectToServer(sock, server_addr);

    // * SOCKET DATA SEND
    sock::client::sendTo(sock, nickname);

    std::thread receiverThread(receiveMessages, sock);
    std::thread senderThread(sendMessages, sock);

    receiverThread.join();
    senderThread.join();

    // close(sock);

    return EXIT_SUCCESS;
}