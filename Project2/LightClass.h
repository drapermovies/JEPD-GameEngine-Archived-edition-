#pragma once
#include <directxmath.h>

class LightClass
{
public:
	LightClass() = default;
	LightClass(const LightClass&) = default;
	~LightClass() = default;

	void SetAmbientColour(float, float, float, float);
	void SetDiffuseColour(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularColour(DirectX::XMFLOAT4);
	void SetSpecularPower(float);

	DirectX::XMFLOAT4 GetAmbientColour();
	DirectX::XMFLOAT4 GetDiffuseColour();
	DirectX::XMFLOAT3 GetDirection();
	DirectX::XMFLOAT4 GetSpecularColour() const;
	float GetSpecularPower() const;
private:
	DirectX::XMFLOAT4 m_ambientColour;
	DirectX::XMFLOAT4 m_diffuseColour;
	DirectX::XMFLOAT3 m_direction;
	DirectX::XMFLOAT4 m_specularColour;
	float m_specularPower;
};