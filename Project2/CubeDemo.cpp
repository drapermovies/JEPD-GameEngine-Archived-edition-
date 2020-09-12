#include "CubeDemo.h"

CubeDemo::CubeDemo(TimerClass& TimerClass, LightShaderClass& LightShader) :
	Scene(TimerClass, LightShader)
{}

void CubeDemo::Shutdown()
{
	for (GameObject* go : m_gameObjects)
	{
		if (go != nullptr)
		{
			delete go;
			go = nullptr;
		}
	}

	for (LightClass* light : m_lightSources)
	{
		if (light != nullptr)
		{
			delete light;
			light = nullptr;
		}
	}

	if (m_camera != nullptr)
	{
		delete m_camera;
		m_camera = nullptr;
	}
}

bool CubeDemo::Initialize()
{
	bool result = false;
	m_camera = new CameraClass();
	m_camera->SetPosition(0.0f, 3.0f, -6.0f);
	m_camera->SetRotation(25.0f, 0.0f, -6.0f);

	GameObject* m_Object = new GameObject();
	if (!m_Object)
	{
		return false;
	}

	result = m_Object->Initialize(m_directX->GetDevice(), m_directX->GetDeviceContext(),
										(char*)"../Project2/Assets/cube.txt",
										(WCHAR*)"../Project2/Assets/stone01.tga");
	if (!result)
	{
		return false;
	}

	LightClass* m_light = new LightClass;
	if (!m_light)
	{
		return false;
	}

	m_light->SetAmbientColour(0.15f, 0.15f, 0.15f, 1.0f);
	m_light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetDirection(1.0f, 0.0f, 0.0f);

	m_Object->SetPosition(0, -1, 3);
	m_Object->SetScale(1.5f, 1.5f, 1.5f);

	m_lightSources.push_back(m_light);
	m_gameObjects.push_back(m_Object);

	return result;
}

void CubeDemo::Update()
{
	DirectX::XMFLOAT3 m_cubeRotation = m_gameObjects[0]->GetRotation();
	m_cubeRotation.y += (float)(DirectX::XM_PI * 0.00025f) * m_timer.GetTime();
	if (m_cubeRotation.y > 360.0f)
	{
		m_cubeRotation.y = 0.0f;
	}
	m_gameObjects[0]->SetRotation(m_cubeRotation);
}