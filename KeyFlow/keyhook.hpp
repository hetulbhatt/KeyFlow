#pragma once
#ifndef KEYHOOK_H
#define KEYHOOK_H

#include "keystroke_handler.hpp"

// Callback function that will be invoked whenever a keyboard event is detected by the hook.
LRESULT CALLBACK keyboard_hook_proc(int nCode, WPARAM wParam, LPARAM lParam);

class KeyHook
{
private:
	KeystrokeHandler keystroke_handler;

public:
	KeystrokeHandler& getKeystrokeHandler()
	{
		return this->keystroke_handler;
	}
	// Rsponsible for setting up the keyboard hook using the SetWindowsHookEx() function. 
	// It passes WH_KEYBOARD_LL as the hook type, indicating a low-level keyboard hook. 
	// The keyboard_hook_proc function is passed as the hook procedure. The NULL parameter represents the module handle, and 0 represents the thread identifier. If the hook setup is successful, it returns 0; otherwise, it returns 1.
	int setup_hook()
	{
		HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_hook_proc, NULL, 0);

		if (hook == NULL)
		{
			printf("Failed to set up keyboard hook\n");
			return 1;
		}

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			// TranslateMessage(&msg);
			// DispatchMessage(&msg);
		}

		UnhookWindowsHookEx(hook);

		return 0;
	}

	LRESULT CALLBACK hook_keyboard(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
		{
			KBDLLHOOKSTRUCT* hook_struct = (KBDLLHOOKSTRUCT*)lParam;
			DWORD vk_code = hook_struct->vkCode;

			this->keystroke_handler.handle_keystroke_event(vk_code, 1);
		}
		else if (nCode >= 0 && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
		{
			KBDLLHOOKSTRUCT* hook_struct = (KBDLLHOOKSTRUCT*)lParam;
			DWORD vk_code = hook_struct->vkCode;
			this->keystroke_handler.handle_keystroke_event(vk_code, 0);
		}
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
};
// The purpose of this class is to ensure that only one instance of the KeyHook class is created and accessed throughout the program.
class KeyHookSingleton
{
private:
	KeyHook* hook = nullptr;
	//The constructor initializes the hook member variable by creating a new KeyHook object
	KeyHookSingleton()
	{
		hook = new KeyHook();
	}

public:
	// Provides the only way to access the single instance of the KeyHook class. It returns a reference to the hook member variable of the KeyHookSingleton instanc
	static KeyHook& getInstance()
	{
		static KeyHookSingleton instance;
		return *(instance.hook);
	}

	KeyHookSingleton(const KeyHookSingleton&) = delete;
	KeyHookSingleton& operator=(const KeyHookSingleton&) = delete;

	~KeyHookSingleton()
	{
		delete hook;
	}
};
// Callback function to be used with the keyboard hook. It returns the result of calling the hook_keyboard() function of the KeyHook instance obtained through the KeyHookSingleton instance.
LRESULT CALLBACK keyboard_hook_proc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return KeyHookSingleton::getInstance().hook_keyboard(nCode, wParam, lParam);
}

#endif // KEYHOOK_H
