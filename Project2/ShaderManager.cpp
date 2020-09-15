#include "ShaderManager.h"

bool ShaderManager::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result = false;
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return result;
	}

	result = m_LightShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light Shader object", L"Error", MB_OK);
		return result;
	}

	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	result = m_TextureShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the canvas shader object", L"Error", MB_OK);
		return result;
	}

	m_FontShader = new FontShader;
	if (!m_FontShader)
	{
		return false;
	}

	result = m_FontShader->Initialize(device, hwnd);

	return result;
}

void ShaderManager::Shutdown()
{
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = nullptr;
	}
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = nullptr;
	}
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = nullptr;
	}
}

bool ShaderManager::RenderFontShader(ID3D11DeviceContext* device_context, 
									 int index_count, 
									 DirectX::XMMATRIX world_matrix, 
									 DirectX::XMMATRIX view_matrix, 
									 DirectX::XMMATRIX projection_matrix, 
									 ID3D11ShaderResourceView* texture, 
									 DirectX::XMFLOAT4 colour)
{
	return m_FontShader->Render(device_context, index_count,
							    world_matrix, view_matrix, projection_matrix, 
								texture, colour);
}

bool ShaderManager::RenderLightShader(ID3D11DeviceContext* device_context, 
								      int index_count, 
									  DirectX::XMMATRIX world_matrix, 
									  DirectX::XMMATRIX view_matrix, 
									  DirectX::XMMATRIX projection_matrix, 
									  ID3D11ShaderResourceView* texture, 
									  DirectX::XMFLOAT3 light_dir, 
									  DirectX::XMFLOAT4 diffuse_colour)
{
	return m_LightShader->Render(device_context, index_count, world_matrix, view_matrix,
								projection_matrix, texture, light_dir, diffuse_colour, 
								diffuse_colour, DirectX::XMFLOAT3(0,0,0), diffuse_colour, 1.0f);
}
