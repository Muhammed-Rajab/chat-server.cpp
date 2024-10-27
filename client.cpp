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

    static JoinedEvent fromJson(const json &j)
    {
        JoinedEvent e;

        if (j.contains("nickname") && j["nickname"].is_string())
        {
            e.nickname = j["nickname"];
        }
        else
        {
            throw std::invalid_argument("Invalid or missing 'nickname' field in JSON for JoinedEvent.");
        }

        return e;
    }
};

struct LeftEvent
{
    std::string nickname;

    static LeftEvent fromJson(const json &j)
    {
        LeftEvent e;

        if (j.contains("nickname") && j["nickname"].is_string())
        {
            e.nickname = j["nickname"];
        }
        else
        {
            throw std::invalid_argument("Invalid or missing 'nickname' field in JSON for LeftEvent.");
        }

        return e;
    }
};

struct MessageEvent
{
    std::string sender;
    std::string data;

    static MessageEvent fromJson(const json &j)
    {
        MessageEvent e;

        if (j.contains("sender") && j["sender"].is_string())
        {
            e.sender = j["sender"].get<std::string>();
        }
        else
        {
            throw std::invalid_argument("Invalid or missing 'sender' field in JSON for MessageEvent.");
        }

        if (j.contains("data") && j["data"].is_string())
        {
            e.data = j["data"].get<std::string>();
        }
        else
        {
            throw std::invalid_argument("Invalid or missing 'data' field in JSON for MessageEvent.");
        }

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

        json eventJson = json::parse(buffer);
        Event event = Event::fromJson(eventJson);

        if (event.type == "joined")
        {
            json payloadJSON = json::parse(event.payload);
            JoinedEvent je = JoinedEvent::fromJson(payloadJSON);

            // TODO: pretty print that user joined
            std::cout << je.nickname << " joined the chat\n";
        }
        else if (event.type == "left")
        {
            json payloadJSON = json::parse(event.payload);
            LeftEvent le = LeftEvent::fromJson(payloadJSON);

            // TODO: pretty print that user left
            std::cout << le.nickname << " left the chat\n";
        }
        else if (event.type == "message")
        {
            json payloadJSON = json::parse(event.payload);
            MessageEvent me = MessageEvent::fromJson(payloadJSON);

            // TODO: pretty print user message
            std::cout << me.sender << " said: " << me.data << "\n";
        }
        else
        {
            std::cout << "unknown event received. inspection advised.\n";
            std::cout << buffer << "\n";
        }
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