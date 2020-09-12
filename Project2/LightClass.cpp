#include "LightClass.h"

void LightClass::SetAmbientColour(float red, float green, float blue, float alpha)
{
	m_ambientColour = DirectX::XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetDiffuseColour(float red, float green, float blue, float alpha)
{
	m_diffuseColour = DirectX::XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = DirectX::XMFLOAT3(x, y, z);
	return;
}

void LightClass::SetSpecularColour(DirectX::XMFLOAT4 new_specular)
{
	m_specularColour = new_specular;
	return;
}

void LightClass::SetSpecularPower(float new_power)
{
	m_specularPower = new_power;
	return;
}

DirectX::XMFLOAT4 LightClass::GetAmbientColour()
{
	return m_ambientColour;
}

DirectX::XMFLOAT4 LightClass::GetDiffuseColour()
{
	return m_diffuseColour;
}

DirectX::XMFLOAT3 LightClass::GetDirection()
{
	return m_direction;
}

DirectX::XMFLOAT4 LightClass::GetSpecularColour() const
{
	return m_specularColour;
}

float LightClass::GetSpecularPower() const
{
	return m_specularPower;
}
