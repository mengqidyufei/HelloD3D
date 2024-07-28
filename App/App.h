#pragma once
#include "Window.h"
#include "ChiliTimer.h"

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
	Window mWnd;
	ChiliTimer mTimer;
	std::vector<std::unique_ptr<class Drawable>> mDrawables;
	static constexpr size_t nDrawables = 180;
};

