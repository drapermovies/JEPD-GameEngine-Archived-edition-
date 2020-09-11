#include "cameraclass.h"

CameraClass::CameraClass()
{
	SetPosition(0, 0, 0);
	SetRotation(0, 0, 0);
}

void CameraClass::SetPosition(float x)
{
	m_Position = DirectX::XMFLOAT3(x, x, x);
	return;
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_Position = DirectX::XMFLOAT3(x, y, z);
	return;
}

void CameraClass::SetRotation(float x)
{
	m_Rotation = DirectX::XMFLOAT3(x, x, x);
	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_Rotation = DirectX::XMFLOAT3(x, y, z);
	return;
}

DirectX::XMFLOAT3 CameraClass::GetPosition() const
{
	return m_Position;
}

DirectX::XMFLOAT3 CameraClass::GetRotation() const
{
	return m_Rotation;
}

void CameraClass::Render()
{
	DirectX::XMFLOAT3 up;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 look_at;

	DirectX::XMVECTOR up_vector;
	DirectX::XMVECTOR position_vector;
	DirectX::XMVECTOR look_at_vector;

	float yaw = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;

	DirectX::XMMATRIX rotation_matrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	up_vector = DirectX::XMLoadFloat3(&up);

	position_vector = DirectX::XMLoadFloat3(&m_Position);

	look_at.x = 0.0f;
	look_at.y = 0.0f;
	look_at.z = 1.0f;

	look_at_vector = DirectX::XMLoadFloat3(&look_at);

	float radians = 0.0174532925f;

	pitch = m_Rotation.x * radians;
	yaw = m_Rotation.y * radians;
	roll = m_Rotation.z * radians;

	rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, roll, yaw);

	look_at_vector = DirectX::XMVector3TransformCoord(look_at_vector, rotation_matrix);

	up_vector = DirectX::XMVector3TransformCoord(up_vector, rotation_matrix);

	look_at_vector = DirectX::XMVectorAdd(position_vector, look_at_vector);

	m_viewMatrix = DirectX::XMMatrixLookAtLH(position_vector, look_at_vector, up_vector);

	return;
}

void CameraClass::GetViewMatrix(DirectX::XMMATRIX& view_matrix)
{
	view_matrix = m_viewMatrix;
	return;
}
