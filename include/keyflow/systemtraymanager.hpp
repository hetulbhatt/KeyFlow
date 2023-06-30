#ifndef SYSTEMTRAYMANAGER_H
#define SYSTEMTRAYMANAGER_H
#include <windows.h>
#include <thread>
#include <string>
#include <iostream>
#include <mutex>

class SystemTrayManager
{
public:
    SystemTrayManager(HWND hwnd = NULL, std::string_view tray_icon_name = "KeyFlow",bool hide_to_tray = true);
    ~SystemTrayManager();
    SystemTrayManager (const SystemTrayManager&) = delete;
    SystemTrayManager& operator= (const SystemTrayManager&) = delete;
    SystemTrayManager (SystemTrayManager&&) = delete;
    SystemTrayManager& operator= (SystemTrayManager&&) = delete;
    inline void set_tray_icon(HICON icon);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
    LRESULT CALLBACK WndProc_impl(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void create_tray_icon(HWND hWnd, NOTIFYICONDATA& nid);
    bool message_loop();
    HWND m_hwnd;
    HWND m_parent;
    HMENU m_hMenu;
    HICON m_tray_icon;
    std::mutex m_mutex;
    std::thread m_thread;
    std::string m_window_title;
    enum MenuID
    {
        ID_SHOWHIDE = 20,
        ID_EXIT
    };
};

SystemTrayManager::SystemTrayManager(HWND hwnd,std::string_view tray_icon_name, bool hide_to_tray) 
: m_parent{hwnd}, m_window_title{tray_icon_name}
{
    if(hide_to_tray)
        ShowWindow(hwnd,SW_HIDE);
    m_tray_icon = nullptr;
    //Create a separate worker thread for the system tray icon
    m_thread = std::thread{ &SystemTrayManager::message_loop,this};
}

SystemTrayManager::~SystemTrayManager()
{
    //Wait for thread to finish the job
    if(m_thread.joinable())
        m_thread.join();
}

void SystemTrayManager::set_tray_icon(HICON icon)
{
    // Lock in case of multiple threads trying to write.
    std::scoped_lock lck{m_mutex};
    m_tray_icon = icon;
    SendMessageW(m_hwnd,WM_SETICON,ICON_SMALL,(LPARAM)icon);
}

bool SystemTrayManager::message_loop()
{
    MSG msg {};
    // We need to create an invisible window class so we can send/receive messages to/from it.
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

void SystemTrayManager::create_tray_icon(HWND hWnd,NOTIFYICONDATA& nid)
{
        nid.cbSize = sizeof(nid);
        nid.hWnd = hWnd;
        nid.uID = 0;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.uCallbackMessage = WM_USER;
        nid.hIcon = m_tray_icon != nullptr ? m_tray_icon : LoadIcon(NULL, IDI_APPLICATION);
        std::copy(m_window_title.begin(), m_window_title.end(), std::begin(nid.szTip));
        Shell_NotifyIcon(NIM_ADD, &nid);
        m_hMenu = CreatePopupMenu();
        AppendMenu(m_hMenu, MF_STRING, ID_SHOWHIDE, "Show/Hide");
        AppendMenu(m_hMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(m_hMenu, MF_STRING, ID_EXIT, "Exit");
}

// The whole function purpose is to readdress the messages to some instance of a class.
// We do it that way because WndProc can only be static for winapi to see it.
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
            create_tray_icon(hWnd,nid);
            break;
        case WM_USER:
            if (lParam == WM_LBUTTONDBLCLK) 
            {
                    ShowWindow(m_parent,SW_SHOW);
            } 
            else if (lParam == WM_RBUTTONUP || lParam == WM_LBUTTONUP) 
            {
                POINT pt;
                int xx;
                GetCursorPos(&pt);
                SetForegroundWindow(hWnd);
                SetFocus(hWnd);
                xx = TrackPopupMenu(m_hMenu, TPM_RETURNCMD, pt.x, pt.y, 0, hWnd, NULL);
                if (xx == ID_SHOWHIDE) 
                {
                    // We can probably use PostMessage to show/hide window in the future.
                    // Window will need to handle the message accordingly.
                    if(IsWindowVisible(m_parent))
                         ShowWindow(m_parent,SW_HIDE);
                         //PostMessage(m_parent,*some message*,FALSE,0);  
                    else
                        ShowWindow(m_parent,SW_SHOW); 
                        //PostMessage(m_parent,*some message*,TRUE,0);
                } 
                else if (xx == ID_EXIT) 
                {
                    Shell_NotifyIcon(NIM_DELETE, &nid);// This should be moved to WM_DESTROY after implementation 
                                                       // of the correct application termination.

                    PostMessage(hWnd, WM_DESTROY,0,0);
                    PostMessage(m_parent, WM_CLOSE,0,0);
                    exit(0);   // We call exit for now, because there is no way to tell the application 
                               // to exit elegantly(with PostMessage(parent,WM_CLOSE) for example).
                } 
                else if (xx == 0) 
                {
                    PostMessage(hWnd, WM_LBUTTONDOWN, 0, 0);
                }
            }
            break;
        case WM_DESTROY:
            //Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            break;
        default:
            if (message == RegisterWindowMessage("TaskbarCreated")) 
            {
                SendMessage(hWnd, WM_CREATE, wParam, lParam);
            }
            break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}



#endif //SYSTEMTRAYMANAGER_H