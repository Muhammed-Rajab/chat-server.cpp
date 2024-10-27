#ifndef UTILS_4_CHAT_SERVER_H
#define UTILS_4_CHAT_SERVER_H

#include <iostream>

void checkAndExit(bool condition, char *message, int errorCode = EXIT_FAILURE)
{
    if (condition)
    {
        std::cerr << message;
        exit(errorCode);
    }
}

#endif