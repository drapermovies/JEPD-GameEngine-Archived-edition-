#pragma once
#include <DirectXMath.h>

class CameraClass 
{
public:
	CameraClass();
	CameraClass(const CameraClass&) = default;
	~CameraClass() = default;

	void SetPosition(float);
	void SetPosition(float, float, float);
	void SetRotation(float);
	void SetRotation(float, float, float);

	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMFLOAT3 GetRotation() const;

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&);

private:
	DirectX::XMFLOAT3 m_Position = DirectX::XMFLOAT3(0, 0, 0);

	DirectX::XMFLOAT3 m_Rotation = DirectX::XMFLOAT3(0, 0, 0);

	DirectX::XMMATRIX m_viewMatrix;
};