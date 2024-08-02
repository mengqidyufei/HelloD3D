#include "App.h"
#include "Window.h"
#include "Box.h"
#include "Melon.h"
#include "Pyramid.h"
#include "ChiliMath.h"
#include <sstream>
#include "Surface.h"
#include "GDIPlusManager.h"
#include "Sheet.h"
#include "SkinnedBox.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

GDIPlusManager gdipm;

App::App()
	:
	mWnd(800, 600, L"Hello-D3D")
{
	class Factory
	{
	public:
		Factory(Graphics& gfx)
			:
			gfx(gfx)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			switch (typedist(rng))
			{
			case 0:
				return std::make_unique<Pyramid>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			case 1:
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
					);
			case 2:
				return std::make_unique<Melon>(
					gfx, rng, adist, ddist,
					odist, rdist, longdist, latdist
					);
			case 3:
				return std::make_unique<Sheet>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			case 4:
				return std::make_unique<SkinnedBox>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			default:
				assert(false && "bad drawable type in factory");
				return {};
			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0, 4 };
	};

	Factory f(mWnd.getGraphics());
	mDrawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(mDrawables), nDrawables, f);
	
	const auto s = Surface::FromFile("Images\\kappa50.png");

	mWnd.getGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));	// Õ∂”∞æÿ’Û
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
	auto dt = mTimer.Mark() * speed_factor;
	mWnd.getGraphics().beginFrame(0.07f, 0.0f, 0.12f);

	mWnd.getGraphics().SetCamera(mCamera.GetMatrix());	// …„œÒª˙ ”Õºæÿ’Û
	for (auto& b : mDrawables)
	{
		b->Update(mWnd.mKeyboard.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
		b->Draw(mWnd.getGraphics());
	}

	{
		//static bool showDemoWindow = (true && mWnd.getGraphics().IsImguiEnabled());
		//if (showDemoWindow)
		//	ImGui::ShowDemoWindow(&showDemoWindow);
		if (ImGui::Begin("Simulation Speed"))
		{
			ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			//static char buffer[1024];
			//ImGui::InputText("Butts", buffer, sizeof(buffer));
			ImGui::Text("Status: %s", mWnd.mKeyboard.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING");
		}
		ImGui::End();
		mCamera.SpawnControlWindow();
	}

	mWnd.getGraphics().endFrame();
}