#include "Graphics.h"

Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};		// ������������
	// BufferDesc ����������  0:Ĭ��ֵ
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	sd.SampleDesc.Count = 1;	// ����������ݵĲ���ģʽ
	sd.SampleDesc.Quality = 0;	// ����Ҫ�����
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//�˻������������ڽ����������RenderTarget��
	sd.BufferCount = 1;		// ������Ҫһ���󻺳������ټ���һ���Զ����õ�ǰ��������������������
	sd.OutputWindow = hWnd;		// ���ھ��
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// ����Ч��
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

	// �õ��󻺳���������������0��Ȼ�󴴽���ȾĿ����ͼ������RenderTargetView֮�󣬾Ͳ���Ҫ�󻺳�����
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
