#pragma once
#include "Graphics.h"

class Camera
{
public:
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;		// ���ܻ��ƿ��ƴ���
	void Reset() noexcept;
private:
	float r = 20.0f;		// ����ԭ��ľ���
	float theta = 0.0f;		// ��ԭ����ת�ĽǶȣ����
	float phi = 0.0f;		// ��ԭ����ת�ĽǶȣ�����
	float pitch = 0.0f;		// ������������ת������
	float yaw = 0.0f;		// ������������ת������
	float roll = 0.0f;		// ������������ת������
};