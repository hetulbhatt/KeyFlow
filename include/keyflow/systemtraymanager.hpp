#ifndef SYSTEMTRAYMANAGER_H
#define SYSTEMTRAYMANAGER_H
#include <windows.h>
#include <thread>
#include <string>
#include <iostream>

class SystemTrayManager
{
public:
    SystemTrayManager(HWND hwnd = NULL,bool hide_to_tray = true);
    ~SystemTrayManager();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
    LRESULT CALLBACK WndProc_impl(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    bool add_tray_icon(UINT nID, HICON hIcon, LPTSTR lpszTip);
    bool message_loop();
    HWND m_hwnd;
    HWND m_parent;
    HMENU m_hMenu;
    std::thread m_thread;
    std::string m_window_title {"KeyFlow"};
};

SystemTrayManager::SystemTrayManager(HWND hwnd, bool hide_to_tray) : m_parent{hwnd}
{
    if(hide_to_tray)
    {
        ShowWindow(hwnd,SW_HIDE);
        m_thread = std::thread{ &SystemTrayManager::message_loop,this};
    }
}
SystemTrayManager::~SystemTrayManager()
{
    if(m_thread.joinable())
        m_thread.join();
}
bool SystemTrayManager::message_loop()
{
    MSG msg {};
    WNDCLASS wndclass {};
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = NULL;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = m_window_title.c_str();
    if (!RegisterClass(&wndclass)) {
        return false;
    }

    m_hwnd = CreateWindowEx(WS_EX_TOOLWINDOW, m_window_title.c_str(),  m_window_title.c_str(), 
                        WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
                        CW_USEDEFAULT, NULL, NULL, NULL, this);
    ShowWindow(m_hwnd, SW_HIDE);
    UpdateWindow(m_hwnd);
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

LRESULT CALLBACK SystemTrayManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    SystemTrayManager *this_ptr;

    if (message == WM_CREATE) {
        this_ptr = (SystemTrayManager*) ((CREATESTRUCT*)lParam)->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) this_ptr);
    } else {
        this_ptr = (SystemTrayManager*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    if (this_ptr)
        return this_ptr->WndProc_impl(hWnd, message, wParam, lParam);

    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK SystemTrayManager::WndProc_impl(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 // Window processing module.
    NOTIFYICONDATA nid {};
    switch (message) {
        case WM_CREATE:
            nid.cbSize = sizeof(nid);
            nid.hWnd = hWnd;
            nid.uID = 0;
            nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            nid.uCallbackMessage = WM_USER;
            nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            std::copy(m_window_title.begin(), m_window_title.end(), std::begin(nid.szTip));
            Shell_NotifyIcon(NIM_ADD, &nid);
            m_hMenu = CreatePopupMenu();
            AppendMenu(m_hMenu, MF_STRING, 12, "Show");
            AppendMenu(m_hMenu, MF_SEPARATOR, 0, NULL);
            AppendMenu(m_hMenu, MF_STRING, 13, "Exit");
            break;
        case WM_USER:
            if (lParam == WM_LBUTTONDOWN) {
                    MessageBox(hWnd, TEXT("left click."), m_window_title.c_str(), MB_OK);
            } else if (lParam == WM_RBUTTONUP) {
                POINT pt;
                int xx;
                GetCursorPos(&pt);
                SetForegroundWindow(hWnd);
                xx = TrackPopupMenu(m_hMenu, TPM_RETURNCMD, pt.x, pt.y, 0, hWnd, NULL);
                if (xx == 12) {
                    ShowWindow(m_parent,SW_SHOW);
                } else if (xx == 13) {
                    PostMessage(hWnd, WM_DESTROY,0,0);
                    PostMessage(m_parent, WM_CLOSE,0,0);
                    exit(0);   // we call exit for now, because there is no way to tell application 
                               // to exit ellegantly(with PostMessage(parent,WM_CLOSE) for example)
                } else if (xx == 0) {
                    PostMessage(hWnd, WM_LBUTTONDOWN, 0, 0);
                }
            }
            break;
        case WM_DESTROY:
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            break;
        default:
            if (message == RegisterWindowMessage("TaskbarCreated")) {
                SendMessage(hWnd, WM_CREATE, wParam, lParam);
            }
            break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}



#endif //SYSTEMTRAYMANAGER_H