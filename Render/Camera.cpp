#include "Camera.h"
#include "imgui/imgui.h"

namespace dx = DirectX;

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	// 计算摄像机位置, 绕原点旋转
	const auto pos = dx::XMVector3Transform(dx::XMVectorSet(0.0f, 0.0f, -r, 0.0f),	dx::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f));
	
	// 将摄像机的位置，和对准目标的位置，以及摄像机的向上分量传递进去，再乘上摄像机的旋转方向
	// 我们希望摄像机看向原点，并且旋转前总是顶部向上
	return dx::XMMatrixLookAtLH( pos, dx::XMVectorZero(), dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) *
		dx::XMMatrixRotationRollPitchYaw( pitch, -yaw, roll );
}

void Camera::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("R", &r, 0.0f, 80.0f, "%.1f");
		ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
		ImGui::SliderAngle("Phi", &phi, -89.0f, 89.0f);
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	r = 20.0f;
	theta = 0.0f;
	phi = 0.0f;
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
}
