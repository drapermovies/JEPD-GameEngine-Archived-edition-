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

bool ViewFrustum::CheckCube(float x_centre, float y_centre, float z_centre, float length)
{
	for (int i = 0; i < 6; i++)
	{
		//float dot_product = 
	}
	return true;
}
