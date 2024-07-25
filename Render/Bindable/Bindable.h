#pragma once
#include "Graphics.h"

class Bindable
{
public:
	Bindable() = default;
	virtual ~Bindable() = default;

	virtual void Bind(Graphics& gfx) = 0;

protected:
	static ID3D11Device* GetDevice(Graphics& gfx);
	static ID3D11DeviceContext* GetContext(Graphics& gfx);
	static DxgiInfoManager& GetInfoManager(Graphics& gfx);
};