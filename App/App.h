#pragma once
#include "Window.h"
#include "ChiliTimer.h"
#include "ImGuiManager.h"

/*!
* Application:
* 1¡¢create window
* 2¡¢while(true)
* {
*		peekMessage();
*		doFrame();
* }
*/

class App
{
public:
	App();
	~App();
	int init();

private:
	void doFrame();

private:
	ImGuiManager mImGui;
	Window mWnd;
	ChiliTimer mTimer;
	std::vector<std::unique_ptr<class Drawable>> mDrawables;
	static constexpr size_t nDrawables = 180;
	float speed_factor = 1.0f;
};

