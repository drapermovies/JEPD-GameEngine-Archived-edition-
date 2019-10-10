#include "colorshaderclass.h"

bool ColorShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result = false;
	result = InitializeShader(device, hwnd, L"../Shaders/color.vs", L"../Engine/color.ps");
	return result;
}

void ColorShaderClass::Shutdown()
{
	ShutdownShader();

	return;
}

bool ColorShaderClass::Render(ID3D11DeviceContext* device_context, int index_count, 
						DirectX::XMMATRIX world_matrix, DirectX::XMMATRIX view_matrix, 
												DirectX::XMMATRIX projection_matrix)
{
	bool result = false;

	result = SetShaderParameters(device_context, world_matrix, view_matrix, projection_matrix);
	if (!result)
	{
		return false;
	}
	RenderShader(device_context, index_count);
	return true;

}

bool ColorShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, 
									WCHAR* vs_fileName, WCHAR* ps_filename)
{
	HRESULT result;
	ID3D10Blob* error_message = nullptr;
	ID3D10Blob* vertex_shader_buffer = nullptr;
	ID3D10Blob* pixel_shader_buffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygon_layout[2];
	unsigned int num_elements = 0;
	CD3D11_BUFFER_DESC matrix_buffer_desc;

	result = D3DCompileFromFile(vs_fileName, nullptr, nullptr, "ColorVertexShader",
										"vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
											&vertex_shader_buffer, &error_message);
	if (FAILED(result))
	{
		if (error_message)
		{
			OutputShaderErrorMessage(error_message, hwnd, vs_fileName);
		}
		else
		{
			MessageBox(hwnd, vs_fileName, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	return false;
}
