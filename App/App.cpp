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
	const float color = sin(mTimer.Peek()) / 2.0f + 0.5f;
	//mWnd.getGraphics().clearRenderTargetView(255, 0, 0);
	mWnd.getGraphics().clearRenderTargetView(color, color, 1.0f);
	mWnd.getGraphics().drawTriangle(mTimer.Peek());
	mWnd.getGraphics().endFrame();
}
