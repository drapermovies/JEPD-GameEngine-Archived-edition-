#include "CubeDemo.h"

void CubeDemo::Update()
{
	bool result = false;
	rotation += (float)(DirectX::XM_PI * 0.00025f) * m_timer->GetTime();
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	result = Render(rotation);
	if (!result)
	{
		return false;
	}
}
