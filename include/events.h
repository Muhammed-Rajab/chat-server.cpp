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
};

#endif