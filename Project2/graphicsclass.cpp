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

	m_camera->SetPosition(0.0f, 0.0f, -6.0f);
	//m_camera->SetRotation(0.0f, -1.0f, 0.0f);

	m_model = new ModelClass;
	if (!m_model)
	{
		return false;
	}

	result = m_model->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), 
										(char*)"../x64/Debug/data/stone01.tga");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize model", L"Error", MB_OK);
		return false;
	}

	m_textureShader = new TextureShaderClass;
	if (!m_textureShader)
	{
		return false;
	}

	result = m_textureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Colour Shader", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_textureShader)
	{
		m_textureShader->Shutdown();
		delete m_textureShader;
		m_textureShader = nullptr;
	}
	if (m_model)
	{
		m_model->Shutdown();
		delete m_model;
		m_model = nullptr;
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
	result = Render();
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render()
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

	m_model->Render(m_D3D->GetDeviceContext());

	result = m_textureShader->Render(m_D3D->GetDeviceContext(), m_model->GetIndexCount(),
											world_matrix, view_matrix, projection_matrix,
																	m_model->GetTexture());
	if (!result)
	{
		return false;
	}

	m_D3D->EndScene();
	return true;
}
