#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <string>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM Wparam, LPARAM Lparam);
wchar_t* stringToWchar(const std::string& str);

class Window
{
public:
	Window(Window&) = delete;
	~Window();
	static Window& GetWindowSingleton();
	HWND GetHandle() { return HandleOfThisWindow; }
	HINSTANCE GetHinstance() { return HinstanceOfThisWindow; }
	void CreateWindowClass();
	RECT CreateRect();
	HWND CreateThisWindow(DWORD& Style, RECT& Rect);
	void CleanAndWrite(const std::string& String);
	static int Run();

private:
	Window();
	HWND HandleOfThisWindow;
	HWND Text;
	HINSTANCE HinstanceOfThisWindow;
	const wchar_t* WindowName;
};

Window& Window::GetWindowSingleton()
{
	static Window window;
	return window;
}

Window::Window()
	:WindowName(L"KeyFlow"), HinstanceOfThisWindow(GetModuleHandle(nullptr))
{
	CreateWindowClass();
	DWORD Style = WS_OVERLAPPEDWINDOW;
	RECT Rect = CreateRect();
	AdjustWindowRect(&Rect, Style, false);
	HandleOfThisWindow = CreateThisWindow(Style, Rect);
	Text = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_READONLY, 20, 20, 500, 50,
		HandleOfThisWindow, nullptr, HinstanceOfThisWindow, nullptr);
	ShowWindow(HandleOfThisWindow, true);
}

void Window::CreateWindowClass()
{
	WNDCLASS Window = { 0 };
	Window.lpszClassName = WindowName;
	Window.hInstance = HinstanceOfThisWindow;
	Window.lpfnWndProc = WindowProc;
	Window.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	Window.hCursor = LoadCursor(nullptr, IDC_ARROW);
	RegisterClass(&Window);
}

RECT Window::CreateRect()
{
	int Width = 680;
	int Height = 480;
	RECT Rect = { 250, 250, Rect.left + Width, Rect.top + Height };

	return Rect;

}

HWND Window::CreateThisWindow(DWORD& Style, RECT& Rect)
{
	return CreateWindow(
		WindowName,
		WindowName,
		Style,
		Rect.left,
		Rect.top,
		Rect.right - Rect.left,
		Rect.bottom - Rect.top,
		nullptr,
		nullptr,
		HinstanceOfThisWindow,
		nullptr
	);

}

int Window::Run()
{
	MSG message = { 0 };
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
	return 0;
}

Window::~Window()
{
	UnregisterClass(WindowName, HinstanceOfThisWindow);
}

void Window::CleanAndWrite(const std::string& String)
{
	SetWindowText(Text, L"");
	SetWindowText(Text, stringToWchar(String));
}

wchar_t* stringToWchar(const std::string& str) {

	size_t len = strlen(str.c_str()) + 1;
	if (len == static_cast<size_t>(-1)) {

		return nullptr;
	}
	size_t BufferLen;
	wchar_t* buffer = new wchar_t[len + 1];
	mbstowcs_s(&BufferLen, buffer, len, str.c_str(), len - 1);

	return buffer;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM Wparam, LPARAM Lparam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, Wparam, Lparam);
	}
	return 0;
}

#endif
