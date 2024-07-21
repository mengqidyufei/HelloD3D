#include "ChiliWin.h"
#include "Window.h"
#include <sstream>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPreInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	try
	{	// register window class
		Window wnd(640, 480, L"Hello D3D");
		//Window wnd2(640, 480, L"Hello D3D-2");

		// 消息循环
		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, NULL, 0, 0)) > 0) {
			TranslateMessage(&msg);		// WM_KEYDOWN translate to WM_CHAR, for text input
			DispatchMessage(&msg);

			while (!wnd.mMouse.IsEmpty())
			{
				const auto e = wnd.mMouse.Read();
				if (e->GetType() == Mouse::Event::Type::Move)
				{
					std::ostringstream oss;
					oss << "Mouse pos:(" << e->GetPosX() << "," << e->GetPosY() << ")";
					wnd.setTitle(oss.str().c_str());
				}
			}
		}
		if (gResult == -1)
			return -1;
		return (int)msg.wParam; // 返回退出代码
	}
	catch (const ChiliException& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}