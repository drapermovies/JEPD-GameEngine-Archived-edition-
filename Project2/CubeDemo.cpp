#include "CubeDemo.h"

CubeDemo::CubeDemo(TimerClass& TimerClass, 
				   ShaderManager& ShaderManager) :
	Scene(TimerClass, ShaderManager)
{}

CubeDemo::~CubeDemo()
{
	Shutdown();
}

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

bool CubeDemo::Initialize(float screen_width, float screen_height)
{
	DirectX::XMMATRIX base_view_matrix;

	bool result = false;
	m_camera = new CameraClass();
	if (!m_camera)
	{
		return false;
	}

	m_camera->SetPosition(0.0f, 3.0f, -6.0f);
	m_camera->SetRotation(0, 0.0f, 0);

	//Enables 2D canvas rendering
	m_camera->Render();
	m_camera->GetViewMatrix(base_view_matrix);


	GameObject* m_Object = new GameObject();
	if (!m_Object)
	{
		return false;
	}

	result = m_Object->Initialize(m_directX->GetDevice(), m_directX->GetDeviceContext(),
										(char*)"../x64/Debug/data/cube.txt",
										(char*)"../x64/Debug/data/stone01.tga");
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
	m_light->SetDirection(0.0f, 0.0f, 1.0f);
	m_light->SetSpecularColour(DirectX::XMFLOAT4(1, 1, 1, 1));
	m_light->SetSpecularPower(32.0f);

	m_Object->SetPosition(0, -1, 3);
	m_Object->SetScale(1.5f, 1.5f, 1.5f);

	BitmapClass* m_bitmap = new BitmapClass;
	if (!m_bitmap)
	{
		return false;
	}

	result = m_bitmap->Initialize(m_directX->GetDevice(),
		m_directX->GetDeviceContext(),
		screen_width, screen_height,
		(WCHAR*)"../x64/Debug/data/stone01.tga",
		256, 256);

	m_lightSources.push_back(m_light);
	m_gameObjects.push_back(m_Object);

	m_UI = new UserInterface();
	if (m_UI)
	{
		result = m_UI->Initialize(m_directX, screen_height, screen_width);
	}

	return result;
}

void CubeDemo::Update()
{
	//DirectX::XMFLOAT3 m_cubeRotation = m_gameObjects[0]->GetRotation();
	//m_cubeRotation.y += (float)(DirectX::XM_PI * 0.00025f) * m_timer.GetTime();
	//if (m_cubeRotation.y > 360.0f)
	//{
	//	m_cubeRotation.y = 0.0f;
	//}
	//m_gameObjects[0]->SetRotation(m_cubeRotation);
}