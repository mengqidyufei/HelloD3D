#pragma once
#include "ChiliWin.h"
#include <d3d11.h>

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void endFrame();	// 将后缓冲区画面翻转给前缓冲区
	void clearRenderTargetView(float red, float green, float blue) noexcept;

private:
	ID3D11Device* mDevice = nullptr;
	IDXGISwapChain* mSwapChain = nullptr;
	ID3D11DeviceContext* mContext = nullptr;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
};

