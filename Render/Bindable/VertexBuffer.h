#pragma once
#include "Bindable.h"
#include "ChiliWin.h"
#include "ChiliException.h"

class VertexBuffer : public Bindable
{
public:
	template <class T>
	VertexBuffer(Graphics& gfx, const std::vector<T> &vertices)
		: mStride(sizeof(T))
	{
		INFOMAN(gfx);
		// ����Buffer�Ǹ�ʲô�õ�
		D3D11_BUFFER_DESC descVertex;
		descVertex.ByteWidth = UINT(sizeof(T) * vertices.size());;
		descVertex.Usage = D3D11_USAGE_DEFAULT;
		descVertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		descVertex.CPUAccessFlags = 0u;
		descVertex.MiscFlags = 0u;
		descVertex.StructureByteStride = sizeof(T);
		// ���������Buffer������
		D3D11_SUBRESOURCE_DATA subDataVertex;
		subDataVertex.pSysMem = vertices.data();
		subDataVertex.SysMemPitch = 0u;
		subDataVertex.SysMemSlicePitch = 0u;
		// ����ID3D11Buffer
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&descVertex, &subDataVertex, &mVertexBuffer));
	}

	void Bind(Graphics& gfx) override;

private:
	UINT mStride;
	wrl::ComPtr<ID3D11Buffer> mVertexBuffer;
};