#include <iostream>
#include "client.h"

#define PORT 8080

int main()
{

    int sock = sock::client::initialize();

    sockaddr_in server_addr = sock::client::getSocketAddrIn(PORT, "127.0.0.1");

    sock::client::connectToServer(sock, server_addr);

    std::string nickname = "rajab";

    // FIRST SEND NICKNAME
    // sock::client::sendTo(sock, nickname.c_str());
    sock::client::sendTo(sock, "hi\n");

    char buffer[1024];
    sock::client::readFrom(sock, buffer, 1024);
    std::cout << "from server: " << buffer;

    close(sock);

    return EXIT_SUCCESS;
}