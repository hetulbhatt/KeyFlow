#ifndef LOGGER_H
#define LOGGER_H

#include <direct.h>
#include <iostream>
#include <fstream>
#include <filesystem>

class Logger
{
private:
    static std::ofstream logFile;

public:
    // static void initialize(const std::string &filePathy)
    // {
    //     std::string dirName = "logs";
    //     std::string fileName = "logs/execution.log";

    //     _mkdir(dirName.c_str());

    //     logFile.open(fileName);
    //     if (!logFile.is_open())
    //     {
    //         std::cerr << "Failed to open the log file for writing!" << std::endl;
    //     }
    // }

    static void initialize()
    {
        std::string dirName = "logs";
        std::string fileName = "logs/execution.log";

        // Create the directory if it doesn't exist
        int result = _mkdir(dirName.c_str());
        if (result != 0)
        {
            if (errno != EEXIST)
            {
                std::cerr << "Failed to create the logs directory!" << std::endl;
            }
        }

        logFile.open(fileName);
        if (!logFile.is_open())
        {
            std::cerr << "Failed to open the log file for writing!" << std::endl;
        }
    }

    static void shutdown()
    {
        logFile.close();
    }

    static void log(const std::string &message)
    {
        // std::cout << message;
        logFile << message;
        logFile.flush();
    }
};

std::ofstream Logger::logFile; // Definition of the static member

#endif
