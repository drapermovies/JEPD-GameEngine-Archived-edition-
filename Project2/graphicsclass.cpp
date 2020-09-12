#include "graphicsclass.h"
#include "CubeDemo.h"

bool GraphicsClass::Initialize(int screen_width, int screen_height, HWND hwnd)
{
	bool result = false; 
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		//Direct X failed to init
		return false;
	}

	result = m_D3D->Initialize(screen_width, screen_height, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX3D", L"Error", MB_OK);
		return false;
	}

	m_timer = new TimerClass;
	if (!m_timer)
	{
		return false;
	}

	result = m_timer->Initialize();
	if (!result)
	{
		return false;
	}

	m_SceneManager = std::make_unique<SceneManager>();
	if (!m_SceneManager)
	{
		return false;
	}

	m_SceneManager->directX = m_D3D;

	LightShaderClass* m_lightShader = new LightShaderClass;
	if (!m_lightShader)
	{
		return false;
	}

	result = m_lightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light Shader object", L"Error", MB_OK);
		return false;
	}

	CubeDemo* DemoScene = new CubeDemo(*m_timer, *m_lightShader);
	m_SceneManager->AddScene(DemoScene);

	DemoScene->Initialize(); //WE GET THE SCENE MANAGERS DX INSTANCE

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = nullptr;
	}
	return;
}

bool GraphicsClass::Frame()
{
	bool result = false;
	static float rotation = 0.0f;

	m_SceneManager->Update();
	m_timer->Frame();

	//rotation += (float)(DirectX::XM_PI * 0.00025f) * m_timer->GetTime();
	//if (rotation > 360.0f)
	//{
	//	rotation -= 360.0f;
	//}

	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render(float rotation)
{
	bool result = false;

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	result = m_SceneManager->Render();

	if (!result)
	{
		return false;
	}

	m_D3D->EndScene();
	return true;
}
