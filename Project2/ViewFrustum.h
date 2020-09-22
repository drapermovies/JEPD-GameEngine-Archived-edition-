/*
* DICTATES WHAT THE CAMERA CAN SEE, AND THEN RENDERS IT. So we only render the stuff we need to see.
*/
#pragma once
#include <DirectXMath.h>

class ViewFrustum
{
public:
	ViewFrustum() = default;
	ViewFrustum(const ViewFrustum&) = default;
	~ViewFrustum() = default;

	void Initialize(float);

	void ConstructViewFrustum(DirectX::XMMATRIX, DirectX::XMMATRIX);

	bool CheckPoint(float, float, float);
	bool CheckCube(float, float, float, float);
	bool CheckSphere(float, float, float, float);
	bool CheckRectangle(float, float, float, float, float, float);

protected:
	struct Plane
	{
		float a;
		float b;
		float c;
		float d;

		void Normalise(float length)
		{
			a /= length;
			b /= length;
			c /= length;
			d /= length;
		}
	};
private:
	float CubeDotProduct(Plane& Plane, float XValue, float YValue, float ZValue);
	float m_screenDepth = -1;
	Plane m_Planes[6];
};