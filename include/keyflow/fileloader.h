#include "helper.h"

#include <fstream>
#include <sstream>

#define DEFAULT_CODE_LENGTH 6

class FileLoader
{
private:
    Helper helper;
    std::vector<std::pair<std::vector<int>, std::string>> dollar_shortcuts;
    std::vector<std::pair<std::vector<int>, std::string>> colon_shortcuts;
    std::unordered_map<std::string, std::string> dollar_codes;
    std::unordered_map<std::string, std::string> colon_codes;
    int code_length = DEFAULT_CODE_LENGTH;

public:
    FileLoader()
    {
        std::cout << "FileLoader()\n";
    }

    std::vector<std::pair<std::vector<int>, std::string>> &getDollarShortcuts()
    {
        return this->dollar_shortcuts;
    }

    std::vector<std::pair<std::vector<int>, std::string>> &getColonShortcuts()
    {
        return this->colon_shortcuts;
    }

    std::unordered_map<std::string, std::string> &getDollarCodes()
    {
        return this->dollar_codes;
    }

    std::unordered_map<std::string, std::string> &getColonCodes()
    {
        return this->colon_codes;
    }

    int getCodeLength()
    {
        return code_length;
    }

    int load_shortcuts()
    {
        std::ifstream file("../config/shortcuts.config");
        std::string line;

        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
            {
                continue;
            }

            std::stringstream ss(line);
            std::string keyCombination, value;
            bool is_dollar = false, is_colon = false;
            if (line.find('$') != std::string::npos)
            {
                is_dollar = true;
                std::getline(ss, keyCombination, '$');
                std::getline(ss, value);
            }
            else if (line.find(':') != std::string::npos)
            {
                is_colon = true;
                std::getline(ss, keyCombination, ':');
                std::getline(ss, value);
            }

            keyCombination = helper.trim(keyCombination);
            value = helper.trim(value);

            std::vector<std::string> splitStrings = helper.split_string(keyCombination, '+');
            std::vector<int> splitInts = helper.convert_strings_to_ints(splitStrings);

            if (!splitInts.empty() && !value.empty())
            {
                if (is_dollar)
                {
                    dollar_shortcuts.push_back(std::make_pair(splitInts, value));
                }
                else if (is_colon)
                {
                    colon_shortcuts.push_back(std::make_pair(splitInts, value));
                }
            }
        }

        std::cout << "Dollar Shortcuts:\n";
        for (const auto &entry : dollar_shortcuts)
        {
            for (auto &num : entry.first)
            {
                std::cout << num << ", ";
            }
            std::cout << " : " << entry.second;
            std::cout << std::endl;
        }
        std::cout << std::endl;

        std::cout << "Colon Shortcuts:\n";
        for (const auto &entry : colon_shortcuts)
        {
            for (auto &num : entry.first)
            {
                std::cout << num << ", ";
            }
            std::cout << " : " << entry.second;
            std::cout << std::endl;
        }
        std::cout << std::endl;

        return 0;
    }

    int load_codes()
    {
        std::ifstream file("../config/codes.config");
        std::string line;

        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
                continue;

            if (helper.starts_with(line, "CODE LENGTH"))
            {
                std::stringstream ss(line);
                std::string code_length_declarator, actual_length;
                std::getline(ss, code_length_declarator, '=');
                std::getline(ss, actual_length);
                try
                {
                    code_length = std::stoi(helper.trim(actual_length));
                }
                catch (const std::invalid_argument &e)
                {
                    code_length = DEFAULT_CODE_LENGTH;
                }
                continue;
            }

            std::stringstream ss(line);
            std::string keyCombination, value;
            bool is_dollar = false, is_colon = false;
            if (line.find('$') != std::string::npos)
            {
                is_dollar = true;
                std::getline(ss, keyCombination, '$');
                std::getline(ss, value);
            }
            else if (line.find(':') != std::string::npos)
            {
                is_colon = true;
                std::getline(ss, keyCombination, ':');
                std::getline(ss, value);
            }

            keyCombination = helper.trim(keyCombination);
            value = helper.trim(value);

            if (keyCombination.size() != code_length)
            {
                continue;
            }

            if (!keyCombination.empty() && !value.empty())
            {
                if (is_dollar)
                {
                    dollar_codes[keyCombination] = value;
                }
                else if (is_colon)
                {
                    colon_codes[keyCombination] = value;
                }
            }
        }

        std::cout << "Dollar Code:\n";
        for (const auto &entry : dollar_codes)
        {
            std::cout << entry.first << " : " << entry.second << std::endl;
        }
        std::cout << std::endl;

        std::cout << "Colon Code:\n";
        for (const auto &entry : colon_codes)
        {
            std::cout << entry.first << " : " << entry.second << std::endl;
        }
        std::cout << std::endl;

        return 0;
    }
};
