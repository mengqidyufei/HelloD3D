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
	int init();

private:
	void doFrame();

private:
	Window mWnd;
	ChiliTimer mTimer;
};

