#include "ChiliWin.h"
#include "Window.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPreInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// register window class
	Window wnd(640, 480, L"Hello D3D");
	//Window wnd2(640, 480, L"Hello D3D-2");

	// 消息循环
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, NULL, 0, 0)) > 0) {
		TranslateMessage(&msg);		// WM_KEYDOWN translate to WM_CHAR, for text input
		DispatchMessage(&msg);
	}
	if (gResult == -1)
		return -1;
	return (int)msg.wParam; // 返回退出代码
}