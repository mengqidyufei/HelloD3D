#include "Window.h"
#include "WindowsMessageMap.h"
#include <ostream>
#include <sstream>

Window::WindowClass Window::WindowClass::wndClass;
LPCWSTR Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.mInstance;
}

Window::WindowClass::WindowClass() noexcept
	:
	mInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = reinterpret_cast<WNDPROC>(HandleMsgSetup);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, mInstance);
}

Window::Window(int width, int height, LPCWSTR name) noexcept
	: mWidth(width)
	, mHeight(height)
{
	// window size
	RECT rect;
	rect.left = 100;
	rect.right = width + rect.left;
	rect.top = 100;
	rect.bottom = height + rect.top;
	AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	// create and show window
	mWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);		// this: 使得Window此窗口实例与HandleMsgSetup绑定，可以在静态函数中cast lParam指针访问此窗口
	ShowWindow(mWnd, SW_SHOWDEFAULT);
	UpdateWindow(mWnd);          // 绘制窗口
}

Window::~Window()
{
	DestroyWindow(mWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_NCCREATE:		// is no non-client region
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(hWnd));	// 类似于 setUserData
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));	// 传递给HandleMsgThunk
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));	// 类似于 getUserData
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

// 真正处理消息的函数：
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	static WindowsMessageMap map;
	std::string msgString = map(msg, lParam, wParam);
	//OutputDebugStringA(msgString.c_str());
	switch (msg)
	{
	case WM_CLOSE:
		//PostQuitMessage(69);		// 直接退出程序
		//break;
		PostQuitMessage(0);			// 走析构函数
		return 0;					// 不走默认的关闭窗口DefWindowProc
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
