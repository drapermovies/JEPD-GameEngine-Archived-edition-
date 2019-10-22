#include "graphicsclass.h"

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

	m_camera = new CameraClass;
	if (!m_camera)
	{
		return false;
	}

	m_camera->SetPosition(0.0f, 3.0f, -6.0f);
	m_camera->SetRotation(25.0f, 0.0f, -6.0f);

	m_object = new GameObject();
	if (!m_object)
	{
		return false;
	}

	result = m_object->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), 
										(char*)"../x64/Debug/data/cube.txt",
										(WCHAR*)"../x64/Debug/data/stone01.tga");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize model", L"Error", MB_OK);
		return false;
	}

	customer = new Customer;
	if (!customer)
	{
		return false;
	}

	m_object->SetName("Game Object");

	m_lightShader = new LightShaderClass;
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

	m_light = new LightClass;
	if (!m_light)
	{
		return false;
	}

	m_light->SetAmbientColour(0.15f, 0.15f, 0.15f, 1.0f);
	m_light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetDirection(1.0f, 0.0f, 0.0f);

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_light)
	{
		delete m_light;
		m_light = nullptr;
	}
	if (m_lightShader)
	{
		m_lightShader->Shutdown();
		delete m_lightShader;
		m_lightShader = nullptr;
	}
	if (m_object)
	{
		m_object->Release();
		delete m_object;
		m_object = nullptr;
	}
	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr;
	}
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
	rotation += (float)DirectX::XM_PI * 0.00025f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render(float rotation)
{
	DirectX::XMMATRIX world_matrix;
	DirectX::XMMATRIX view_matrix;
	DirectX::XMMATRIX projection_matrix;
	bool result = false;

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->Render();

	m_D3D->GetWorldMatrix(world_matrix);
	m_camera->GetViewMatrix(view_matrix);
	m_D3D->GetProjectionMatrix(projection_matrix);

	world_matrix = DirectX::XMMatrixRotationY(rotation);

	m_object->Render(m_D3D->GetDeviceContext());

	result = m_lightShader->Render(m_D3D->GetDeviceContext(), 
								   m_object->GetModel()->GetIndexCount(),
								   world_matrix, view_matrix, projection_matrix,
								   m_object->GetModel()->GetTexture(),
								   m_light->GetDirection(),
								   m_light->GetAmbientColour(),
					    		   m_light->GetDiffuseColour());
	if (!result)
	{
		return false;
	}

	m_D3D->EndScene();
	return true;
}
