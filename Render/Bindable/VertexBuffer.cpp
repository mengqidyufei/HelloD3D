#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& gfx)
{
	// ��������װ����
	UINT offset = 0u;
	// �˴���GetAddressOf����Ϊ���˴����ǲ���Ҫ��vertexBuffer����κζ�����ֻ��ָ��ָ���ַ��
	// ��ComPtr��&���Ļ��ͻ��֮ǰ�������ͷŵ�������������Ҫ�Ľ����������GetAdressOf����ֻȡ��ַ
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, mVertexBuffer.GetAddressOf(), &mStride, &offset);
}