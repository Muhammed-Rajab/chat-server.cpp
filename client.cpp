#include <iostream>
#include <thread>

#include "client.h"
#include "events.h"
#include "external/json.hpp"

#define PORT 8080

using json = nlohmann::json;

struct JoinedEvent
{
    std::string nickname;

    static JoinedEvent fromJson(const nlohmann::json &j)
    {
        JoinedEvent e;
        j.at("nickname").get_to(e.nickname);
        return e;
    }
};

struct LeftEvent
{
    std::string nickname;

    static LeftEvent fromJson(const nlohmann::json &j)
    {
        LeftEvent e;
        j.at("nickname").get_to(e.nickname);
        return e;
    }
};

struct MessageEvent
{
    std::string sender;
    std::string data;

    static MessageEvent fromJson(const nlohmann::json &j)
    {
        MessageEvent e;
        j.at("sender").get_to(e.sender);
        j.at("data").get_to(e.data);
        return e;
    }
};

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

        // TODO: take the buffer, parse it to event, and do stuff
        json eventJson = json::parse(buffer);
        Event event = Event::fromJson(eventJson);

        if (event.type == "joined")
        {
            JoinedEvent je = JoinedEvent::fromJson(event.payload);
        }
        else if (event.type == "left")
        {
            LeftEvent le = LeftEvent::fromJson(event.payload);
        }
        else if (event.type == "message")
        {
            MessageEvent me = MessageEvent::fromJson(event.payload);
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
        std::string event = Event{
            "message",
            message}
                                .toJson()
                                .dump(2);

        if (sock::client::sendTo(socket_fd, event) == -1)
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