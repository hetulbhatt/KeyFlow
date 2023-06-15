#include "keystroke_handler.h"

LRESULT CALLBACK keyboard_hook_proc(int nCode, WPARAM wParam, LPARAM lParam);

class KeyHook
{
private:
    KeystrokeHandler keystroke_handler;

public:
    KeystrokeHandler &getKeystrokeHandler()
    {
        return this->keystroke_handler;
    }

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
            KBDLLHOOKSTRUCT *hook_struct = (KBDLLHOOKSTRUCT *)lParam;
            DWORD vk_code = hook_struct->vkCode;

            this->keystroke_handler.handle_keystroke_event(vk_code, 1);
        }
        else if (nCode >= 0 && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
        {
            KBDLLHOOKSTRUCT *hook_struct = (KBDLLHOOKSTRUCT *)lParam;
            DWORD vk_code = hook_struct->vkCode;
            this->keystroke_handler.handle_keystroke_event(vk_code, 0);
        }
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
};

class KeyHookSingleton
{
private:
    KeyHook *hook = nullptr;

    KeyHookSingleton()
    {
        hook = new KeyHook();
    }

public:
    static KeyHook &getInstance()
    {
        static KeyHookSingleton instance;
        return *(instance.hook);
    }

    KeyHookSingleton(const KeyHookSingleton &) = delete;
    KeyHookSingleton &operator=(const KeyHookSingleton &) = delete;

    ~KeyHookSingleton()
    {
        delete hook;
    }
};

LRESULT CALLBACK keyboard_hook_proc(int nCode, WPARAM wParam, LPARAM lParam)
{
    return KeyHookSingleton::getInstance().hook_keyboard(nCode, wParam, lParam);
}
