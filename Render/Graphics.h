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
	void beginFrame(float red, float green, float blue) noexcept;

	void DrawIndexed(UINT count);
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void SetCamera(DirectX::FXMMATRIX camera) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;

	void EnableImgui() noexcept;
	void DisableImgui() noexcept;
	bool IsImguiEnabled() const noexcept;

private:
	bool mEnableImGui = true;
	DirectX::XMMATRIX mProjection;		// 投影矩阵
	DirectX::XMMATRIX mCamera;			// 摄像机视图矩阵

private:
	wrl::ComPtr<ID3D11Device> mDevice;
	wrl::ComPtr<IDXGISwapChain> mSwapChain;
	wrl::ComPtr<ID3D11DeviceContext> mContext;
	wrl::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	DxgiInfoManager infoManager;

	friend class Bindable;
};

