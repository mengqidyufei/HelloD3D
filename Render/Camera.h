#pragma once
#include "Graphics.h"

class Camera
{
public:
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;		// 繁衍绘制控制窗口
	void Reset() noexcept;
private:
	float r = 20.0f;		// 距离原点的距离
	float theta = 0.0f;		// 绕原点旋转的角度，赤道
	float phi = 0.0f;		// 绕原点旋转的角度，经线
	float pitch = 0.0f;		// 摄像机自身的旋转，上下
	float yaw = 0.0f;		// 摄像机自身的旋转，左右
	float roll = 0.0f;		// 摄像机自身的旋转，翻滚
};