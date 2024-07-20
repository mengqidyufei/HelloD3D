#pragma once

#include <unordered_map>
#include "ChiliWin.h"

class WindowsMessageMap
{
public:
	WindowsMessageMap();
	std::string operator()(DWORD msg, LPARAM lp, WPARAM wp) const;

private:
	std::unordered_map<DWORD, std::string> map;
};

