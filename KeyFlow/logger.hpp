#pragma once
#ifndef LOGGER_H
#define LOGGER_H
#include <direct.h>// Provides access to functions for working with directories
#include <iostream>
#include <fstream>
#include <filesystem>// Provides access to file and directory operations

class Logger
{
private:
	static std::ofstream logFile;// This member variable will be used to write log messages to a file

public:
	// This static member function is used to initialize the logger. It creates a directory named "logs" (if it doesn't already exist) using the _mkdir() function from <direct.h>.
	// It then opens the file "logs/execution.log" for writing using the open() function of logFile.
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

	static void shutdown()//It closes the logFile 
	{
		logFile.close();
	}

	static void log(const std::string& message)
	{
		std::cout << message;
		logFile << message;
		logFile.flush();
	}
};

std::ofstream Logger::logFile; // Definition of the static member

#endif
