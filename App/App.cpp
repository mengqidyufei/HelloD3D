#include "App.h"
#include "Window.h"
#include "Box.h"
#include <sstream>

App::App()
    : 
	mWnd(800, 600, L"Hello-D3D")
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 1.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.08f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 180; i++)
	{
		mBoxs.push_back(std::make_unique<Box>(
			mWnd.getGraphics(), rng, adist,
			ddist, odist, rdist
			));
	}
	mWnd.getGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
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

App::~App()
{}

void App::doFrame()
{
	auto dt = mTimer.Mark();
	mWnd.getGraphics().clearRenderTargetView(0.07f, 0.0f, 0.12f);
	for (auto& b : mBoxs)
	{
		b->Update(dt);
		b->Draw(mWnd.getGraphics());
	}
	mWnd.getGraphics().endFrame();
}