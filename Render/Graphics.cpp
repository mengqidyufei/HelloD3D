#include "Graphics.h"
#include "ChiliException.h"
#include <d3dcompiler.h>

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

void Graphics::drawTriangle()
{
	// ׼��һ������
	struct Vertex
	{
		float x;
		float y;
	};
	const Vertex vertices[] =
	{
		{ 0.0f, 0.5f },
		{ 0.5f, -0.5f },
		{ -0.5f, -0.5f },
	};
	// ����Buffer�Ǹ�ʲô�õ�
	D3D11_BUFFER_DESC desc;				
	desc.ByteWidth = sizeof(vertices);
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0u;
	desc.MiscFlags = 0u;
	desc.StructureByteStride = sizeof(Vertex);

	// ���������Buffer������
	D3D11_SUBRESOURCE_DATA subData;		
	subData.pSysMem = vertices;
	subData.SysMemPitch = 0u;
	subData.SysMemSlicePitch = 0u;

	// ����ID3D11Buffer
	wrl::ComPtr<ID3D11Buffer> vertexBuffer;
	HRESULT hr;
	GFX_THROW_INFO( mDevice->CreateBuffer(&desc, &subData, &vertexBuffer) );

	// ��������װ����
	UINT pStrides = sizeof(Vertex);
	UINT offset = 0u;
	// �˴���GetAddressOf����Ϊ���˴����ǲ���Ҫ��vertexBuffer����κζ�����ֻ��ָ��ָ���ַ��
	// ��ComPtr��&���Ļ��ͻ��֮ǰ�������ͷŵ�������������Ҫ�Ľ����������GetAdressOf����ֻȡ��ַ
	mContext->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &pStrides, &offset);

	// ����VertexShader
	wrl::ComPtr<ID3DBlob> blobVS;
	wrl::ComPtr<ID3D11VertexShader> vertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &blobVS));
	GFX_THROW_INFO(mDevice->CreateVertexShader(blobVS->GetBufferPointer(), blobVS->GetBufferSize(), nullptr, &vertexShader));
	mContext->VSSetShader(vertexShader.Get(), nullptr, 0);

	// ����ͼԪ����
	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ���������ݴ���VS��
	wrl::ComPtr<ID3D11InputLayout> inputLayout;
	const D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mDevice->CreateInputLayout(inputDesc, (UINT)std::size(inputDesc), blobVS->GetBufferPointer(), blobVS->GetBufferSize(), &inputLayout);
	mContext->IASetInputLayout(inputLayout.Get());


	wrl::ComPtr<ID3DBlob> blobPS;
	// ����pixelShader
	wrl::ComPtr<ID3D11PixelShader> pixelShader;
	// �˴��ٴ�ʹ��blob����Ϊ��ComPtr�����ͷ�ԭ�е����ݣ�Ȼ��Ż���Ϊ�����ݵ�ָ��ȥ����µ�����
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &blobPS));
	GFX_THROW_INFO(mDevice->CreatePixelShader(blobPS->GetBufferPointer(), blobPS->GetBufferSize(), nullptr, &pixelShader));
	mContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	// ��ȾĿ����ͼ
	// �˴���GetAddressOf����Ϊ���˴����ǲ���Ҫ��RenderTargetView����κζ�����ֻ��ָ��ָ���ַ��
	// ��ComPtr��&���Ļ��ͻ��֮ǰ�������ͷŵ�������������Ҫ�Ľ����������GetAdressOf����ֻȡ��ַ
	mContext->OMSetRenderTargets(1u, mRenderTargetView.GetAddressOf(), nullptr);


	// NDC -> viewport
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	mContext->RSSetViewports(1u, &viewport);
	//GFX_THROW_INFO_ONLY(mContext->Draw(3u, 0u));
	mContext->Draw(3u, 0u);
}