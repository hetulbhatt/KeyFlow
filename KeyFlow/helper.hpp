#pragma once
#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <unordered_map>
#include <string>

class Helper
{
public:
	std::unordered_map<std::string, int> key_to_vkcode;
	//constrctor
	Helper()
	{
		init_key_to_vkcode();//intialized the unordered_map
		Logger::log("Helper()\n");
	}
	//removes the removes leading and trailing spaces from str
	static std::string trim(const std::string& str)
	{
		size_t first = str.find_first_not_of(' ');
		if (std::string::npos == first)
		{
			return str;
		}
		size_t last = str.find_last_not_of(' ');
		return str.substr(first, (last - first + 1));
	}
	// takes a std::string and a delimiter character as input and splits the string into multiple substrings based on the delimiter. 
	//It uses a std::vector<std::string> to store the resulting substrings
	static std::vector<std::string> split_string(const std::string& str, char delimiter)
	{
		std::vector<std::string> tokens;
		std::size_t start = 0;
		std::size_t end = str.find(delimiter);

		while (end != std::string::npos)
		{
			tokens.push_back(str.substr(start, end - start));
			start = end + 1;
			end = str.find(delimiter, start);
		}

		tokens.push_back(str.substr(start));
		return tokens;
	}
	//This function takes a vector of strings as input and converts each string to an integer value based on the key_to_vkcode map
	std::vector<int> convert_strings_to_ints(std::vector<std::string> strings)
	{
		std::vector<int> result;
		for (const std::string& str : strings)
		{
			if (key_to_vkcode.find(str) != key_to_vkcode.end())
			{
				result.push_back(key_to_vkcode[str]);
			}
			else
			{
				// logger
			}
		}
		return result;
	}
	//checks if fullString starts with prefix
	static bool starts_with(const std::string& fullString, const std::string& prefix)
	{
		if (fullString.length() < prefix.length())
		{
			return false;
		}
		return fullString.substr(0, prefix.length()) == prefix;
	}
	//nitializes the key_to_vkcode unordered map using an initializer list. 
	//The initializer list contains a series of key-value pairs enclosed in curly braces {}.
	//Each key-value pair corresponds to a key (represented as a std::string) and a value (represented as an int) according to the table in the Readmy
	void init_key_to_vkcode()
	{
		key_to_vkcode = {
			{"LBUTTON", 1},
			{"RBUTTON", 2},
			{"CANCEL", 3},
			{"MBUTTON", 4},
			{"XBUTTON1", 5},
			{"XBUTTON2", 6},
			{"BACK", 8},
			{"TAB", 9},
			{"CLEAR", 12},
			{"RETURN", 13},
			{"SHIFT", 16},
			{"CONTROL", 17},
			{"MENU", 18},
			{"PAUSE", 19},
			{"CAPITAL", 20},
			{"KANA", 21},
			{"HANGUEL", 21},
			{"HANGUL", 21},
			{"IME_ON", 22},
			{"JUNJA", 23},
			{"FINAL", 24},
			{"HANJA", 25},
			{"KANJI", 25},
			{"IME_OFF", 26},
			{"ESCAPE", 27},
			{"CONVERT", 28},
			{"NONCONVERT", 29},
			{"ACCEPT", 30},
			{"MODECHANGE", 31},
			{"SPACE", 32},
			{"PRIOR", 33},
			{"NEXT", 34},
			{"END", 35},
			{"HOME", 36},
			{"LEFT", 37},
			{"UP", 38},
			{"RIGHT", 39},
			{"DOWN", 40},
			{"SELECT", 41},
			{"PRINT", 42},
			{"EXECUTE", 43},
			{"SNAPSHOT", 44},
			{"INSERT", 45},
			{"DELETE", 46},
			{"HELP", 47},
			{"0", 48},
			{"1", 49},
			{"2", 50},
			{"3", 51},
			{"4", 52},
			{"5", 53},
			{"6", 54},
			{"7", 55},
			{"8", 56},
			{"9", 57},
			{"A", 65},
			{"B", 66},
			{"C", 67},
			{"D", 68},
			{"E", 69},
			{"F", 70},
			{"G", 71},
			{"H", 72},
			{"I", 73},
			{"J", 74},
			{"K", 75},
			{"L", 76},
			{"M", 77},
			{"N", 78},
			{"O", 79},
			{"P", 80},
			{"Q", 81},
			{"R", 82},
			{"S", 83},
			{"T", 84},
			{"U", 85},
			{"V", 86},
			{"W", 87},
			{"X", 88},
			{"Y", 89},
			{"Z", 90},
			{"LWIN", 91},
			{"RWIN", 92},
			{"APPS", 93},
			{"-", 94},
			{"SLEEP", 95},
			{"NUMPAD0", 96},
			{"NUMPAD1", 97},
			{"NUMPAD2", 98},
			{"NUMPAD3", 99},
			{"NUMPAD4", 100},
			{"NUMPAD5", 101},
			{"NUMPAD6", 102},
			{"NUMPAD7", 103},
			{"NUMPAD8", 104},
			{"NUMPAD9", 105},
			{"MULTIPLY", 106},
			{"ADD", 107},
			{"SEPARATOR", 108},
			{"SUBTRACT", 109},
			{"DECIMAL", 110},
			{"DIVIDE", 111},
			{"F1", 112},
			{"F2", 113},
			{"F3", 114},
			{"F4", 115},
			{"F5", 116},
			{"F6", 117},
			{"F7", 118},
			{"F8", 119},
			{"F9", 120},
			{"F10", 121},
			{"F11", 122},
			{"F12", 123},
			{"F13", 124},
			{"F14", 125},
			{"F15", 126},
			{"F16", 127},
			{"F17", 128},
			{"F18", 129},
			{"F19", 130},
			{"F20", 131},
			{"F21", 132},
			{"F22", 133},
			{"F23", 134},
			{"F24", 135},
			{"NUMLOCK", 144},
			{"SCROLL", 145},
			{"LSHIFT", 160},
			{"RSHIFT", 161},
			{"LCONTROL", 162},
			{"RCONTROL", 163},
			{"LMENU", 164},
			{"RMENU", 165},
			{"BROWSER_BACK", 166},
			{"BROWSER_FORWARD", 167},
			{"BROWSER_REFRESH", 168},
			{"BROWSER_STOP", 169},
			{"BROWSER_SEARCH", 170},
			{"BROWSER_FAVORITES", 171},
			{"BROWSER_HOME", 172},
			{"VOLUME_MUTE", 173},
			{"VOLUME_DOWN", 174},
			{"VOLUME_UP", 175},
			{"MEDIA_NEXT_TRACK", 176},
			{"MEDIA_PREV_TRACK", 177},
			{"MEDIA_STOP", 178},
			{"MEDIA_PLAY_PAUSE", 179},
			{"LAUNCH_MAIL", 180},
			{"LAUNCH_MEDIA_SELECT", 181},
			{"LAUNCH_APP1", 182},
			{"LAUNCH_APP2", 183},
			{"OEM_1", 186},
			{"OEM_PLUS", 187},
			{"OEM_COMMA", 188},
			{"OEM_MINUS", 189},
			{"OEM_PERIOD", 190},
			{"OEM_2", 191},
			{"OEM_3", 192},
			{"OEM_4", 219},
			{"OEM_5", 220},
			{"OEM_6", 221},
			{"OEM_7", 222},
			{"OEM_8", 223},
			{"OEM_102", 226},
			{"PROCESSKEY", 229},
			{"PACKET", 231},
			{"ATTN", 246},
			{"CRSEL", 247},
			{"EXSEL", 248},
			{"EREOF", 249},
			{"PLAY", 250},
			{"ZOOM", 251},
			{"NONAME", 252},
			{"PA1", 253},
			{"OEM_CLEAR", 254} };
	}
};

#endif // HELPER_H
