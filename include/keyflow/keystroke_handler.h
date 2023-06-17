#ifndef KEYSTROKE_HANDLER_H
#define KEYSTROKE_HANDLER_H

#include "action_performer.h"
#include "fileloader.h"

#define TOTAL_KEYS 256
#define PASTE_EVENT 1
#define EXECUTE_EVENT 2

class KeystrokeHandler
{
private:
    FileLoader loader;
    ActionPerformer action_performer;

    int logs[TOTAL_KEYS];
    int sequence;
    std::vector<int> buffer;
    int buffer_index = 0;
    std::unordered_map<std::string, int> key_to_vkcode;

public:
    KeystrokeHandler()
    {
        this->sequence = 0;
        memset(logs, 0, TOTAL_KEYS * sizeof(int));
        printf("KeystrokeHandler() %d \n", loader.getCodeLength());
    }

    FileLoader &getFileLoader()
    {
        return this->loader;
    }

    void resizeBuffer(int size)
    {
        this->buffer.resize(size);
    }

    void execute_shortcut()
    {
        this->execute_shortcut(PASTE_EVENT);
        this->execute_shortcut(EXECUTE_EVENT);
    }

    void execute_shortcut(int event_type)
    {
        std::vector<std::pair<std::vector<int>, std::string>> &shortcuts =
            event_type == PASTE_EVENT ? loader.getColonShortcuts() : loader.getDollarShortcuts();

        for (const auto &entry : shortcuts)
        {
            std::vector<int> list = entry.first;
            for (int i = 0; i < list.size(); i++)
            {
                int key = list[i];
                std::cout << std::endl
                          << key << " " << logs[key] << " " << i + 1;
                if (logs[key] != i + 1)
                {
                    break;
                }
                if (i == list.size() - 1)
                {
                    for (int j = 0; j < list.size(); j++)
                    {
                        logs[list[j]] = 0;
                        sequence--;
                    }
                    if (event_type == PASTE_EVENT)
                    {
                        action_performer.simulate_paste(entry.second);
                    }
                    else if (event_type == EXECUTE_EVENT)
                    {
                        action_performer.execute_program(entry.second);
                    }
                }
            }
        }
    }

    void execute_code(char c)
    {
        int codeLength = loader.getCodeLength();
        buffer[buffer_index] = c;
        buffer_index = (buffer_index + 1) % codeLength;
        std::string key;
        int idx = buffer_index;
        for (int i = 0; i < codeLength; i++)
        {
            key += buffer[idx];
            idx = (idx + 1) % codeLength;
        }
        this->execute_code(PASTE_EVENT, key);
        this->execute_code(EXECUTE_EVENT, key);
    }

    void execute_code(int event_type, std::string key)
    {
        std::unordered_map<std::string, std::string> &codes =
            event_type == PASTE_EVENT ? loader.getColonCodes() : loader.getDollarCodes();
        if (codes.find(key) != codes.end())
        {
            std::cout << key << std::endl;
            if (event_type == PASTE_EVENT)
            {
                action_performer.simulate_paste(codes[key]);
            }
            else if (event_type == EXECUTE_EVENT)
            {
                action_performer.execute_program(codes[key]);
            }
        }
    }

    void handle_keystroke_event(DWORD vk_code, int isDown)
    {
        if (isDown)
        {
            std::cout << vk_code << std::endl;
            this->execute_code((char)vk_code);
            if (logs[(int)vk_code] == 0)
            {
                std::cout << vk_code << std::endl;
                sequence++;
                logs[(int)vk_code] = sequence;
                this->execute_shortcut();
            }
        }
        else
        {
            if (logs[(int)vk_code] > 0)
            {
                logs[vk_code] = 0;
                --sequence;
            }
        }
    }
};

#endif // KEYSTROKE_HANDLER_H
