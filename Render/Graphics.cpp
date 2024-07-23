#include "Graphics.h"
#include "ChiliException.h"
#include <d3dcompiler.h>

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

	// 得到后缓冲区（纹理，索引是0）然后创建渲染目标视图，有了RenderTargetView之后，就不需要后缓冲区了
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
	// 准备一组数据
	struct Vertex
	{
		float x;
		float y;

		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};
	const Vertex vertices[] =
	{
		{ 0.0f, 0.5f, 255, 0, 0 },
		{ 0.5f, -0.5f, 0, 255, 0 },
		{ -0.5f, -0.5f, 0, 0, 255 },

		//{ 0.0f, 0.5f, 255, 0, 0 },
		//{ -0.5f, -0.5f, 0, 255, 0 },
		{ -0.3f, 0.3f, 0, 0, 255 },

		//{ 0.0f, 0.5f, 255, 0, 0 },
		{ 0.3f, 0.3f, 0, 255, 0 },
		//{ 0.5f, -0.5f, 0, 0, 255 },

		{ 0.0f, -0.8f, 255, 0, 0 },
		//{ -0.5f, -0.5f, 0, 255, 0 },
		//{ 0.5f, -0.5f, 0, 0, 255 },
	};
	{
		// 描述Buffer是干什么用的
		D3D11_BUFFER_DESC descVertex;				
		descVertex.ByteWidth = sizeof(vertices);
		descVertex.Usage = D3D11_USAGE_DEFAULT;
		descVertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		descVertex.CPUAccessFlags = 0u;
		descVertex.MiscFlags = 0u;
		descVertex.StructureByteStride = sizeof(Vertex);

		// 真正输入给Buffer的数据
		D3D11_SUBRESOURCE_DATA subDataVertex;		
		subDataVertex.pSysMem = vertices;
		subDataVertex.SysMemPitch = 0u;
		subDataVertex.SysMemSlicePitch = 0u;

		// 创建ID3D11Buffer
		wrl::ComPtr<ID3D11Buffer> vertexBuffer;
		HRESULT hr;
		GFX_THROW_INFO( mDevice->CreateBuffer(&descVertex, &subDataVertex, &vertexBuffer) );

		// 设置输入装配器
		UINT pStrides = sizeof(Vertex);
		UINT offset = 0u;
		// 此处用GetAddressOf是因为，此处我们不需要给vertexBuffer填充任何东西，只是指定指针地址，
		// 而ComPtr用&符的话就会把之前的内容释放掉，不是我们想要的结果，所以用GetAdressOf仅仅只取地址
		mContext->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &pStrides, &offset);
	}

	// 顶点索引
	const unsigned short indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 1,
		5, 2, 1,
	};
	// 设置Buffer类型为顶点索引，以及步进量
	D3D11_BUFFER_DESC descIndexBuffer = {};
	descIndexBuffer.ByteWidth = sizeof(indices);
	descIndexBuffer.Usage = D3D11_USAGE_DEFAULT;
	descIndexBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
	descIndexBuffer.CPUAccessFlags = 0u;
	descIndexBuffer.MiscFlags = 0u;
	descIndexBuffer.StructureByteStride = sizeof(unsigned short);

	// 真正输入给Buffer的数据
	D3D11_SUBRESOURCE_DATA subDataIndexBuffer = {};
	subDataIndexBuffer.pSysMem = indices;
	subDataIndexBuffer.SysMemPitch = 0u;
	subDataIndexBuffer.SysMemSlicePitch = 0u;

	// 创建ID3D11Buffer
	wrl::ComPtr<ID3D11Buffer> vertexIndexBuffer;
	HRESULT hr;
	GFX_THROW_INFO(mDevice->CreateBuffer(&descIndexBuffer, &subDataIndexBuffer, &vertexIndexBuffer));
	mContext->IASetIndexBuffer(vertexIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// 加载VertexShader
	wrl::ComPtr<ID3DBlob> blobVS;
	wrl::ComPtr<ID3D11VertexShader> vertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &blobVS));
	GFX_THROW_INFO(mDevice->CreateVertexShader(blobVS->GetBufferPointer(), blobVS->GetBufferSize(), nullptr, &vertexShader));
	mContext->VSSetShader(vertexShader.Get(), nullptr, 0);

	// 设置图元拓扑
	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 将顶点数据传入VS中
	wrl::ComPtr<ID3D11InputLayout> inputLayout;
	const D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0},		// 8: 2个float
	};
	mDevice->CreateInputLayout(inputDesc, (UINT)std::size(inputDesc), blobVS->GetBufferPointer(), blobVS->GetBufferSize(), &inputLayout);
	mContext->IASetInputLayout(inputLayout.Get());


	wrl::ComPtr<ID3DBlob> blobPS;
	// 加载pixelShader
	wrl::ComPtr<ID3D11PixelShader> pixelShader;
	// 此处再次使用blob是因为，ComPtr会先释放原有的内容，然后才会作为空内容的指针去填充新的内容
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &blobPS));
	GFX_THROW_INFO(mDevice->CreatePixelShader(blobPS->GetBufferPointer(), blobPS->GetBufferSize(), nullptr, &pixelShader));
	mContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	// 渲染目标视图
	// 此处用GetAddressOf是因为，此处我们不需要给RenderTargetView填充任何东西，只是指定指针地址，
	// 而ComPtr用&符的话就会把之前的内容释放掉，不是我们想要的结果，所以用GetAdressOf仅仅只取地址
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
	//mContext->Draw((UINT)std::size(vertices), 0u);

	mContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}