#include "Graphics.h"

Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};		// 交换链描述符
	// BufferDesc 缓存描述符  0:默认值
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	sd.SampleDesc.Count = 1;	// 用于消除锯齿的采样模式
	sd.SampleDesc.Quality = 0;	// 不需要抗锯齿
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//此缓冲区内容用于交换后输出到RenderTarget上
	sd.BufferCount = 1;		// 我们需要一个后缓冲区，再加上一个自动设置的前缓冲区，即两个缓冲区
	sd.OutputWindow = hWnd;		// 窗口句柄
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// 交换效果
	sd.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&mSwapChain,
		&mDevice,
		nullptr,
		&mContext
	);

	// 得到后缓冲区（纹理，索引是0）然后创建渲染目标视图，有了RenderTargetView之后，就不需要后缓冲区了
	ID3D11Resource* BackBUffer = nullptr;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&BackBUffer));
	mDevice->CreateRenderTargetView(BackBUffer, nullptr, &mRenderTargetView);
	BackBUffer->Release();
}

Graphics::~Graphics()
{
	if (mDevice)
		mDevice->Release();
	if (mSwapChain)
		mSwapChain->Release();
	if (mContext)
		mContext->Release();
	if (mRenderTargetView)
		mRenderTargetView->Release();
}

void Graphics::endFrame()
{
	mSwapChain->Present(1, 0);
}

void Graphics::clearRenderTargetView(float red, float green, float blue) noexcept
{
	const float color[] = { red, green, blue, 1.0f };
	mContext->ClearRenderTargetView(mRenderTargetView, color);
}
