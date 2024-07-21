#include "App.h"
#include "Window.h"
#include <sstream>

App::App()
    : 
	mWnd(800, 600, L"Hello-D3D")
{
}

int App::init()
{
	while (true)
	{
		if (std::optional<int> ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		doFrame();
	}
}

void App::doFrame()
{
	const float timer = mTimer.Peek();
	std::ostringstream oss;
	oss << "Timer elapsed:" << std::setprecision(1) << std::fixed << timer << "s";
	mWnd.setTitle(oss.str().c_str());
}
