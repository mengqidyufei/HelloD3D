#include "Bindable.h"

ID3D11Device* Bindable::GetDevice(Graphics& gfx)
{
	return gfx.mDevice.Get();
}

ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx)
{
	return gfx.mContext.Get();
}

DxgiInfoManager& Bindable::GetInfoManager(Graphics& gfx)
{
	return gfx.infoManager;
}
