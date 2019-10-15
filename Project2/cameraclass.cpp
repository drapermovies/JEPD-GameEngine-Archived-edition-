#include "cameraclass.h"

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

DirectX::XMFLOAT3 CameraClass::GetPosition()
{
	return DirectX::XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

DirectX::XMFLOAT3 CameraClass::GetRotation()
{
	return DirectX::XMFLOAT3(m_rotationX, m_positionY, m_positionZ);
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

	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	position_vector = DirectX::XMLoadFloat3(&position);

	look_at.x = 0.0f;
	look_at.y = 0.0f;
	look_at.z = 1.0f;

	look_at_vector = DirectX::XMLoadFloat3(&look_at);

	float radians = 0.0174532925f;

	pitch = m_rotationX * radians;
	yaw = m_rotationY * radians;
	roll = m_rotationZ * radians;

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
