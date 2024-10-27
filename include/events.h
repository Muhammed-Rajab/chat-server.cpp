#ifndef EVENTS_4_CHAT_SERVER_H
#define EVENTS_4_CHAT_SERVER_H

#include <string>
#include "external/json.hpp"

struct Event
{
    std::string type;
    std::string payload;

    nlohmann::json toJson()
    {
        return {
            {"type", this->type},
            {"payload", this->payload},
        };
    }

    static Event fromJson(const nlohmann::json &j)
    {
        Event event;

        // Check if "type" exists and is a string
        if (j.contains("type") && j["type"].is_string())
        {
            event.type = j["type"].get<std::string>();
        }
        else
        {
            throw std::invalid_argument("Invalid or missing 'type' field in JSON.");
        }

        // Check if "payload" exists and is a string
        if (j.contains("payload") && j["payload"].is_string())
        {
            event.payload = j["payload"].get<std::string>();
        }
        else
        {
            throw std::invalid_argument("Invalid or missing 'payload' field in JSON.");
        }

        return event;
    }
};

#endif