#pragma once
#include "Window.h"
#include "ChiliTimer.h"

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

