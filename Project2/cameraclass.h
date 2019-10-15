#pragma once
#include <DirectXMath.h>

class CameraClass 
{
public:
	CameraClass() = default;
	CameraClass(const CameraClass&) = default;
	~CameraClass() = default;

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&);

private:
	float m_positionX = 0.0f;
	float m_positionY = 0.0f;
	float m_positionZ = 0.0f;

	float m_rotationX = 0.0f;
	float m_rotationY = 0.0f;
	float m_rotationZ = 0.0f;

	DirectX::XMMATRIX m_viewMatrix;
};