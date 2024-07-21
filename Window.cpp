#include "Window.h"
#include "WindowsMessageMap.h"
#include "resource.h"
#include <ostream>
#include <sstream>

Window::Window(int width, int height, LPCWSTR name)
	: mWidth(width)
	, mHeight(height)
	, mMouse(Mouse())
	, mKeyboard(Keyboard())
{
	// window size
	RECT rect;
	rect.left = 100;
	rect.right = width + rect.left;
	rect.top = 100;
	rect.bottom = height + rect.top;
	if (false == AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE))
	{
		throw CHWND_LAST_EXCEPT();
	}
	// create and show window
	mWnd = CreateWindow(
		WindowClass::GetWndClassName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);		// this: 使得Window此窗口实例与HandleMsgSetup绑定，可以在静态函数中cast lParam指针访问此窗口
	if (nullptr == mWnd)
	{
		throw CHWND_LAST_EXCEPT();
	}
	ShowWindow(mWnd, SW_SHOWDEFAULT);
	UpdateWindow(mWnd);          // 绘制窗口
}

Window::~Window()
{
	DestroyWindow(mWnd);
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
	case WM_KILLFOCUS:
		mKeyboard.ClearState();		// 当弹窗后，键盘焦点不在主窗口就清空键盘所有状态
		break;
	case WM_KEYDOWN:
		// Alt key is WM_SYSKEYDOWN / WM_SYSKEYUP
		// 0x4000-0000: MSDN上查WM_KEYDOWN的lParam解释，处理持续按下某个键后，只有第30位可变化，比如加速前进Shift+W
		if (!(lParam & 0x40000000) || mKeyboard.AutorepeatIsEnabled())
		{
			mKeyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
		mKeyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		mKeyboard.OnChar(static_cast<unsigned char>(wParam));
		break;
	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < mWidth && pt.y >= 0 && pt.y < mHeight)
		{
			mMouse.OnMouseMove(pt.x, pt.y);
			if (!mMouse.IsInWindow())
			{
				SetCapture(mWnd);
				mMouse.OnMouseEnter();
			}
		}
		else
		{
			if (wParam && (MK_LBUTTON | MK_RBUTTON))
			{
				mMouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mMouse.OnMouseLeave();
			}
		}
	}
		break;
	case WM_LBUTTONDOWN:
	{
		POINTS pt = MAKEPOINTS(lParam);
		mMouse.OnLeftPressed(pt.x, pt.y);
	}
	break;
	case WM_RBUTTONDOWN:
	{
		POINTS pt = MAKEPOINTS(lParam);
		mMouse.OnRightPressed(pt.x, pt.y);
	}
	break;
	case WM_LBUTTONUP:
	{
		POINTS pt = MAKEPOINTS(lParam);
		mMouse.OnLeftReleased(pt.x, pt.y);
	}
	break;
	case WM_RBUTTONUP:
	{
		POINTS pt = MAKEPOINTS(lParam);
		mMouse.OnRightReleased(pt.x, pt.y);
	}
	break;
	case WM_MOUSEWHEEL:
	{
		POINTS pt = MAKEPOINTS(lParam);
		mMouse.OnWheelDelta(pt.x, pt.y, GET_WHEEL_DELTA_WPARAM(wParam));
	}
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::setTitle(const char*text)
{
	if (false == SetWindowTextA(mWnd, text))
	{
		throw CHWND_LAST_EXCEPT();
	}
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}
		TranslateMessage(&msg);		// WM_KEYDOWN translate to WM_CHAR, for text input
		DispatchMessage(&msg);
	}
	return std::optional<int>();
}

Window::WindowClass Window::WindowClass::wndClass;
LPCWSTR Window::WindowClass::GetWndClassName() noexcept
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
	wc.hIcon = static_cast<HICON>(LoadImage(mInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetWndClassName();
	wc.hIconSm = static_cast<HICON>(LoadImage(mInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, mInstance);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_NCCREATE:		// is no non-client region
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));	// 类似于 setUserData
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

// Window Exception Stuff
std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	hr(hr)
{}

const char* Window::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::HrException::GetType() const noexcept
{
	return "Chili Window Exception";
}

HRESULT Window::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::HrException::GetErrorDescription() const noexcept
{
	return Exception::TranslateErrorCode(hr);
}


const char* Window::NoGfxException::GetType() const noexcept
{
	return "Chili Window Exception [No Graphics]";
}