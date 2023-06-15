#include <windows.h>

#include <string>
#include <iostream>

class ActionPerformer
{
public:
    ActionPerformer() {
        std::cout << "ActionPerformer()\n";
    }

    void simulate_paste(const std::string &stringToPaste)
    {
        // Get the handle of the active window
        HWND activeWindow = GetForegroundWindow();

        // Set the active window as the foreground window
        SetForegroundWindow(activeWindow);

        // Set up the datastructure
        INPUT *inputs = new INPUT[stringToPaste.length() * 2];
        // ZeroMemory(inputs, stringToPaste.length() * 2 * sizeof(INPUT));
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

    void execute_program(const std::string &path)
    {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

        if (!CreateProcess(NULL, const_cast<char *>(path.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
        {
            std::cerr << "Failed to create process: " << GetLastError() << std::endl;
            return;
        }

        // // Wait for the process to finish
        // WaitForSingleObject(pi.hProcess, INFINITE);

        // Close process and thread handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        std::cout << "Process execution completed for: " << path << std::endl;
    }
};
