#pragma once
#include "ChiliWin.h"
#include "DxgiInfoManager.h"

class Bindable;
class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void endFrame();	// 将后缓冲区画面翻转给前缓冲区
	void clearRenderTargetView(float red, float green, float blue) noexcept;

	void DrawIndexed(UINT count);
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
private:
	DirectX::XMMATRIX projection;

private:
	wrl::ComPtr<ID3D11Device> mDevice;
	wrl::ComPtr<IDXGISwapChain> mSwapChain;
	wrl::ComPtr<ID3D11DeviceContext> mContext;
	wrl::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	DxgiInfoManager infoManager;

	friend class Bindable;
};

