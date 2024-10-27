#include <iostream>
#include "client.h"

#define PORT 8080

int main()
{

    int sock = sock::client::initialize();

    sockaddr_in server_addr = sock::client::getSocketAddrIn(PORT, "127.0.0.1");

    sock::client::connectToServer(sock, server_addr);

    sock::client::sendMessage(sock, "working fine\n");

    close(sock);

    return EXIT_SUCCESS;
}