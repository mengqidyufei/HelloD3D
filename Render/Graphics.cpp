#include "Graphics.h"
#include "ChiliException.h"

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

	UINT swapCreateFlags = D3D11_CREATE_DEVICE_DEBUG;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&mSwapChain,
		&mDevice,
		nullptr,
		&mContext
	);
	if (hr != HRESULT())
	{
		GFX_THROW_INFO(hr);
	}

	// �õ��󻺳���������������0��Ȼ�󴴽���ȾĿ����ͼ������RenderTargetView֮�󣬾Ͳ���Ҫ�󻺳�����
	wrl::ComPtr<ID3D11Resource> BackBUffer;
	GFX_THROW_INFO(mSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBUffer));
	GFX_THROW_INFO(mDevice->CreateRenderTargetView(BackBUffer.Get(), nullptr, &mRenderTargetView));
}

Graphics::~Graphics()
{
}

void Graphics::endFrame()
{
	HRESULT hr = mSwapChain->Present(1, 0);
	if (hr != 0)
	{
		infoManager.reset();
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(mDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	}
}

void Graphics::clearRenderTargetView(float red, float green, float blue) noexcept
{
	const float color[] = { red, green, blue, 1.0f };
	mContext->ClearRenderTargetView(mRenderTargetView.Get(), color);
}
