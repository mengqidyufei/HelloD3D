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
	);		// this: ʹ��Window�˴���ʵ����HandleMsgSetup�󶨣������ھ�̬������cast lParamָ����ʴ˴���
	ShowWindow(mWnd, SW_SHOWDEFAULT);
	UpdateWindow(mWnd);          // ���ƴ���
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
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(hWnd));	// ������ setUserData
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));	// ���ݸ�HandleMsgThunk
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));	// ������ getUserData
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

// ����������Ϣ�ĺ�����
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	static WindowsMessageMap map;
	std::string msgString = map(msg, lParam, wParam);
	//OutputDebugStringA(msgString.c_str());
	switch (msg)
	{
	case WM_CLOSE:
		//PostQuitMessage(69);		// ֱ���˳�����
		//break;
		PostQuitMessage(0);			// ����������
		return 0;					// ����Ĭ�ϵĹرմ���DefWindowProc
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
