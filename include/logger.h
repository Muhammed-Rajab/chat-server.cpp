#ifndef LOGGER_4_CHAT_SERVER_H
#define LOGGER_4_CHAT_SERVER_H

#include <string>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

namespace Logger
{

    enum Level
    {
        WARN,
        ERROR,
        DEBUG,
        INFO,
        SUCCESS,
        EVENT
    };

    void log(Level level, const std::string &message)
    {
        auto now = std::time(nullptr);
        std::cout << "\033[1m[" << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << "]";

        switch (level)
        {
        case WARN:
            std::cout << "\033[33m" << "[WARN]";
            break;
        case DEBUG:
            std::cout << "\033[37m" << "[DEBUG]";
            break;
        case ERROR:
            std::cout << "\033[31m" << "[ERROR]";
            break;
        case INFO:
            std::cout << "\033[34m" << "[INFO]";
            break;
        case SUCCESS:
            std::cout << "\033[92m" << "[SUCCESS]";
            break;
        case EVENT:
            std::cout << "\033[35m" << "[EVENT]";
            break;
        }

        std::cout << ": \033[0m" << message << std::endl;
    }

    template <typename... Args>
    void logf(Level level, const std::string &format_str, Args... args)
    {
        int size = std::snprintf(nullptr, 0, format_str.c_str(), args...) + 1;
        if (size <= 0)
        {
            throw std::runtime_error("Formatting error.");
        }

        std::vector<char> buf(size);
        std::snprintf(buf.data(), size, format_str.c_str(), args...);

        log(level, std::string(buf.data(), buf.size() - 1));
    }
}

#endif