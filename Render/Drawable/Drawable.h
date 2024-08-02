#pragma once
#include "ChiliWin.h"
#include <DirectXMath.h>

class Bindable;
class IndexBuffer;
class Graphics;

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;		// 顶点数据变换矩阵
	void Draw(Graphics& gfx) const;
	virtual void Update(float dt) noexcept = 0;
	void AddBind(std::unique_ptr<Bindable> bind);
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;
	virtual ~Drawable() = default;

private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;

private:
	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;

	template <class T>
	friend class DrawableBase;
};