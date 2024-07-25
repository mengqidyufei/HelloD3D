#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& gfx)
{
	// 设置输入装配器
	UINT offset = 0u;
	// 此处用GetAddressOf是因为，此处我们不需要给vertexBuffer填充任何东西，只是指定指针地址，
	// 而ComPtr用&符的话就会把之前的内容释放掉，不是我们想要的结果，所以用GetAdressOf仅仅只取地址
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, mVertexBuffer.GetAddressOf(), &mStride, &offset);
}