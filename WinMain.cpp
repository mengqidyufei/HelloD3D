#include <Windows.h>
#include "WindowsMessageMap.h"

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT msg, WPARAM wParma, LPARAM lParam)
{
	static WindowsMessageMap map;
	std::string msgString = map(msg, lParam, wParma);
	OutputDebugStringA(msgString.c_str());
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		break;
	case WM_KEYDOWN:
		if (wParma == 'F')
		{
			SetWindowText(hWnd, L"Key F down.");
		}
		break;
	case WM_KEYUP:
		if (wParma == 'F')
		{
			SetWindowText(hWnd, L"Key F up.");
		}
		break;
	}
	return DefWindowProc(hWnd, msg, wParma, lParam);
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPreInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = reinterpret_cast<WNDPROC>(MyWndProc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;

	LPCWSTR pClassName = L"hw3d-HelloWorld";
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
	// create window instace
	HWND hWnd = CreateWindowEx(
		0, pClassName, L"Hard window",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, 640, 480,
		nullptr, nullptr, hInstance, nullptr
	);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);          // 绘制窗口
	// 消息循环
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, NULL, 0, 0)) > 0) {
		//if (!TranslateAccelerator(hWnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		//}
	}
	if (gResult == -1)
		return -1;
	return (int)msg.wParam; // 返回退出代码
}