#include "ViewFrustum.h"

void ViewFrustum::Initialize(float screen_depth)
{
	m_screenDepth = screen_depth;
}

void ViewFrustum::ConstructViewFrustum(DirectX::XMMATRIX projection_matrix, 
									   DirectX::XMMATRIX view_matrix)
{
	DirectX::XMFLOAT4X4 p_matrix, matrix;

	XMStoreFloat4x4(&p_matrix, projection_matrix);

	//Calculate Z Distance
	float z_min = -p_matrix._43 / p_matrix._33;
	float r = m_screenDepth / (m_screenDepth - z_min);

	p_matrix._33 = r;
	p_matrix._43 = -r * z_min;
	projection_matrix = DirectX::XMLoadFloat4x4(&p_matrix);

	DirectX::XMMATRIX final_matrix = DirectX::XMMatrixMultiply(view_matrix, projection_matrix);

	DirectX::XMStoreFloat4x4(&matrix, final_matrix);

	//Calculate nearest plane of the viewing frustum
	m_Planes[0].a = matrix._14 + matrix._13;
	m_Planes[0].b = matrix._24 + matrix._23;
	m_Planes[0].c = matrix._34 + matrix._33;
	m_Planes[0].d = matrix._44 + matrix._43;

	//Normalise the nearest plane 
	float length = sqrtf(pow(m_Planes[0].a, 2) + pow(m_Planes[0].b, 2) +
						 pow(m_Planes[0].c, 2) + pow(m_Planes[0].d, 2));

	m_Planes[0].Normalise(length);

	//Calculate the furthest plane of the viewing frustum
	m_Planes[1].a = matrix._14 - matrix._13;
	m_Planes[1].b = matrix._24 - matrix._23;
	m_Planes[1].c = matrix._34 - matrix._33;
	m_Planes[1].d = matrix._44 - matrix._43;

	length = sqrtf(pow(m_Planes[1].a, 2) + pow(m_Planes[1].b, 2) +
		pow(m_Planes[1].c, 2) + pow(m_Planes[1].d, 2));

	m_Planes[1].Normalise(length);

	//Calculate the left plane
	m_Planes[2].a = matrix._14 + matrix._11;
	m_Planes[2].b = matrix._24 + matrix._21;
	m_Planes[2].c = matrix._34 + matrix._31;
	m_Planes[2].d = matrix._44 + matrix._41;

	length = sqrtf(pow(m_Planes[2].a, 2) + pow(m_Planes[2].b, 2) +
		pow(m_Planes[2].c, 2) + pow(m_Planes[2].d, 2));

	m_Planes[2].Normalise(length);

	//Right plane
	m_Planes[3].a = matrix._14 - matrix._11;
	m_Planes[3].b = matrix._24 - matrix._21;
	m_Planes[3].c = matrix._34 - matrix._31;
	m_Planes[3].d = matrix._44 - matrix._41;

	length = sqrtf(pow(m_Planes[3].a, 2) + pow(m_Planes[3].b, 2) +
		pow(m_Planes[3].c, 2) + pow(m_Planes[3].d, 2));

	m_Planes[3].Normalise(length);

	//Top plane
	m_Planes[4].a = matrix._14 - matrix._12;
	m_Planes[4].b = matrix._24 - matrix._22;
	m_Planes[4].c = matrix._34 - matrix._32;
	m_Planes[4].d = matrix._44 - matrix._42;

	length = sqrtf(pow(m_Planes[4].a, 2) + pow(m_Planes[4].b, 2) +
		pow(m_Planes[4].c, 2) + pow(m_Planes[4].d, 2));

	m_Planes[4].Normalise(length);

	//Bottom plane
	m_Planes[5].a = matrix._14 + matrix._12;
	m_Planes[5].b = matrix._24 + matrix._22;
	m_Planes[5].c = matrix._34 + matrix._32;
	m_Planes[5].d = matrix._44 + matrix._42;

	length = sqrtf(pow(m_Planes[5].a, 2) + pow(m_Planes[5].b, 2) +
		pow(m_Planes[5].c, 2) + pow(m_Planes[5].d, 2));

	m_Planes[5].Normalise(length);
}

bool ViewFrustum::CheckPoint(float x, float y, float z)
{
	for (int i = 0; i < 6; i++)
	{
		float dot_product = (m_Planes[i].a * x) + (m_Planes[i].b * y)
							+ (m_Planes[i].c * z) + (m_Planes[i].d * 1.0f);

		if (dot_product <= 0.0f)
		{
			return false;
		}
	}
	return true;
}

bool ViewFrustum::CheckCube(float XCentre, float YCentre, float ZCentre, float Length)
{
	float PositiveXValue = XCentre + Length;
	float NegativeXValue = XCentre - Length;

	float PositiveYValue = YCentre + Length;
	float NegativeYValue = YCentre - Length;

	float PositiveZValue = ZCentre + Length;
	float NegativeZValue = ZCentre - Length;

	float DotProduct = 1.0f;

	//Check each of the frustum planes to see if the cube is insdie it
	for (int face = 0; face < 6; face++)
	{
		float DotProduct = 1;

		//Check all 8 points to see if they are within the viewing frustum
		for (int point = 0; point <= 8; point++)
		{
			float XValue = PositiveXValue, YValue = PositiveYValue, ZValue = PositiveZValue;
			if (DotProduct > 0.0f)
			{
				//All points are in the frustum
				if (point == 8) { return false; }

				if (point % 2 == 0)
				{
					XValue = NegativeXValue;
				}
				if (point == 0 || point == 1 || point == 4 || point == 5)
				{
					YValue = NegativeYValue;
				}
				if (point < 4)
				{
					ZValue = NegativeZValue;
				}

				DotProduct = CubeDotProduct(m_Planes[face], XValue, YValue, ZValue);
			}
		}
	}
	return true;
}

bool ViewFrustum::CheckSphere(float XCentre, float YCentre, float ZCentre, float Radius)
{
	//Check all 6 viewing planes
	for (int i = 0; i < 6; i++)
	{
		float DotProduct = (m_Planes[i].a * XCentre) + (m_Planes[i].b * YCentre)
							+ (m_Planes[i].c * ZCentre) + m_Planes[i].d;

		if (DotProduct <= -Radius)
		{
			return false;
		}
	}
	return true;
}

bool ViewFrustum::CheckRectangle(float XCentre, float YCentre, float ZCentre,
	float XSize, float YSize, float ZSize)
{
	float PositiveXValue = XCentre + XSize;
	float NegativeXValue = XCentre - XSize;

	float PositiveYValue = YCentre + YSize;
	float NegativeYValue = YCentre - YSize;

	float PositiveZValue = ZCentre + ZSize;
	float NegativeZValue = ZCentre - ZSize;

	//Check all 6 planes
	for (short int i = 0; i < 6; i++)
	{
		float DotProduct = 0.0f;
		//Check all 8 points to see if they are within the viewing frustum
		for (int point = 0; point <= 8; point++)
		{
			float XValue = PositiveXValue, YValue = PositiveYValue, ZValue = PositiveZValue;
			if (DotProduct > 0.0f)
			{
				//All points are in the frustum
				if (point == 8) { return false; }

				if (point % 2 == 0)
				{
					XValue = NegativeXValue;
				}
				if (point == 0 || point == 1 || point == 4 || point == 5)
				{
					YValue = NegativeYValue;
				}
				if (point < 4)
				{
					ZValue = NegativeZValue;
				}

				DotProduct = CubeDotProduct(m_Planes[i], XValue, YValue, ZValue);
			}
		}
	}
}

float ViewFrustum::CubeDotProduct(Plane& Plane, float XValue, float YValue, float ZValue)
{
	return ((Plane.a * XValue) + (Plane.b * YValue) + (Plane.d * ZValue) + Plane.d);
}