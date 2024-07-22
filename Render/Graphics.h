#pragma once
#include "ChiliWin.h"
#include "DxgiInfoManager.h"
#include <d3d11.h>
#include <wrl.h>
namespace wrl = Microsoft::WRL;

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void endFrame();	// 将后缓冲区画面翻转给前缓冲区
	void clearRenderTargetView(float red, float green, float blue) noexcept;
	void drawTriangle();

private:
	wrl::ComPtr<ID3D11Device> mDevice;
	wrl::ComPtr<IDXGISwapChain> mSwapChain;
	wrl::ComPtr<ID3D11DeviceContext> mContext;
	wrl::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	DxgiInfoManager infoManager;
};

