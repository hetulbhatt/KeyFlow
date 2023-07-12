#pragma once
#ifndef ACTION_PERFORMER_H
#define ACTION_PERFORMER_H

#include <windows.h>
#include <string>
#include <iostream>

#include "logger.hpp"

class ActionPerformer
{
public:
    ActionPerformer()
    {
        Logger::log("ActionPerformer()\n");
    }

    void simulate_paste(const std::string& stringToPaste)
    {
        // Get the handle of the active window
        HWND activeWindow = GetForegroundWindow();

        // Set the active window as the foreground window
        SetForegroundWindow(activeWindow);

        // Set up the datastructure
        INPUT* inputs = new INPUT[stringToPaste.length() * 2];
        memset(inputs, 0, stringToPaste.length() * 2 * sizeof(INPUT));

        for (size_t i = 0; i < stringToPaste.length(); i++)
        {
            // Set up the INPUT structure for the character
            inputs[i * 2].type = INPUT_KEYBOARD;
            inputs[i * 2].ki.dwFlags = KEYEVENTF_UNICODE;
            inputs[i * 2].ki.wScan = stringToPaste[i];

            inputs[i * 2 + 1].type = INPUT_KEYBOARD;
            inputs[i * 2 + 1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
            inputs[i * 2 + 1].ki.wScan = stringToPaste[i];
        }

        // Send the input events to the active window
        SendInput(stringToPaste.length() * 2, inputs, sizeof(INPUT));

        delete[] inputs;
    }

    void execute_program(const std::string& path)
    {
        STARTUPINFOA startupInfo;
        PROCESS_INFORMATION processInfo;

        ZeroMemory(&startupInfo, sizeof(startupInfo));
        startupInfo.cb = sizeof(startupInfo);

        ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

        if (!CreateProcessA(NULL, const_cast<char*>(path.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo))
        {
            std::cerr << "Failed to create process: " << GetLastError() << std::endl;
            return;
        }

        // // Wait for the process to finish
        // WaitForSingleObject(pi.hProcess, INFINITE);

        // Close process and thread handles
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);

        Logger::log("Process execution completed for: " + path + "\n");
    }
};

#endif
