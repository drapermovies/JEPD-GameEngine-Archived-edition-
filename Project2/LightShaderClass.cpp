#include "LightShaderClass.h"

bool LightShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result = false;

	result = InitializeShader(device, hwnd,
							(WCHAR*)L"../x64/Debug/data/Shaders/Light.vs",
							(WCHAR*)L"../x64/Debug/data/Shaders/Light.ps");
	return result;
}

void LightShaderClass::Shutdown()
{
	ShutdownShader();
	return;
}

bool LightShaderClass::Render(ID3D11DeviceContext* device_context, 
							int index_count, 
							DirectX::XMMATRIX world_matrix, 
							DirectX::XMMATRIX view_matrix, 
							DirectX::XMMATRIX projection_matrix, 
							ID3D11ShaderResourceView* texture, 
							DirectX::XMFLOAT3 light_dir, 
							DirectX::XMFLOAT4 ambient_colour, 
							DirectX::XMFLOAT4 diffuse_colour)
{
	bool result = false;

	result = SetShaderParameters(device_context, world_matrix, view_matrix,
								 projection_matrix, texture, light_dir,
								 ambient_colour, diffuse_colour);
	if (result)
	{
		RenderShader(device_context, index_count);
	}
	return result;
}

bool LightShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd,
	WCHAR* vs_filename, WCHAR* ps_filename)
{
	HRESULT result = S_OK;
	ID3D10Blob* error_message = nullptr;
	ID3D10Blob* vertex_shader_buffer = nullptr;
	ID3D10Blob* pixel_shader_buffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygon_layout[3];
	unsigned int num_elements = 0;
	D3D11_SAMPLER_DESC sampler_desc;
	D3D11_BUFFER_DESC matrix_buffer_desc;
	D3D11_BUFFER_DESC light_buffer_desc;

	result = D3DCompileFromFile(vs_filename, nullptr, nullptr, "LightVertexShader",
		"vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertex_shader_buffer, &error_message);
	if (FAILED(result))
	{
		if (error_message)
		{
			OutputShaderErrorMessage(error_message, hwnd, vs_filename);
		}
		else
		{
			MessageBox(hwnd, vs_filename, L"Missing shader file", MB_OK);
		}
		return false;
	}

	result = D3DCompileFromFile(ps_filename, nullptr, nullptr, "LightPixelShader",
		"ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixel_shader_buffer, &error_message);
	if (FAILED(result))
	{
		if (error_message)
		{
			OutputShaderErrorMessage(error_message, hwnd, ps_filename);
		}
		else
		{
			MessageBox(hwnd, ps_filename, L"Missing shader file", MB_OK);
		}
		return false;
	}

	result = device->CreateVertexShader(vertex_shader_buffer->GetBufferPointer(),
										vertex_shader_buffer->GetBufferSize(),
										nullptr, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(pixel_shader_buffer->GetBufferPointer(),
									   pixel_shader_buffer->GetBufferSize(),
									   nullptr, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	polygon_layout[0].SemanticName = "POSITION";
	polygon_layout[0].SemanticIndex = 0;
	polygon_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygon_layout[0].InputSlot = 0;
	polygon_layout[0].AlignedByteOffset = 0;
	polygon_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[0].InstanceDataStepRate = 0;

	polygon_layout[1].SemanticName = "TEXCOORD";
	polygon_layout[1].SemanticIndex = 0;
	polygon_layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygon_layout[1].InputSlot = 0;
	polygon_layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[1].InstanceDataStepRate = 0;

	polygon_layout[2].SemanticName = "NORMAL";
	polygon_layout[2].SemanticIndex = 0;
	polygon_layout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygon_layout[2].InputSlot = 0;
	polygon_layout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[2].InstanceDataStepRate = 0;

	num_elements = sizeof(polygon_layout) / sizeof(polygon_layout[0]);

	result = device->CreateInputLayout(polygon_layout, num_elements,
									   vertex_shader_buffer->GetBufferPointer(),
		                               vertex_shader_buffer->GetBufferSize(),
									   &m_inputLayout);
	if (FAILED(result))
	{
		return false;
	}

	vertex_shader_buffer->Release();
	vertex_shader_buffer = nullptr;

	pixel_shader_buffer->Release();
	pixel_shader_buffer = nullptr;

	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MipLODBias = 0.0f;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampler_desc.BorderColor[0] = 0;
	sampler_desc.BorderColor[1] = 0;
	sampler_desc.BorderColor[2] = 0;
	sampler_desc.BorderColor[3] = 0;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&sampler_desc, &m_samplerState);
	if (FAILED(result))
	{
		return false;
	}

	matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	matrix_buffer_desc.ByteWidth = sizeof(MatrixBufferType);
	matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrix_buffer_desc.MiscFlags = 0;
	matrix_buffer_desc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrix_buffer_desc, nullptr, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	light_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	light_buffer_desc.ByteWidth = sizeof(LightBufferType);
	light_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	light_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	light_buffer_desc.MiscFlags = 0;
	light_buffer_desc.StructureByteStride = 0;

	result = device->CreateBuffer(&light_buffer_desc, nullptr, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void LightShaderClass::ShutdownShader()
{
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}
	if (m_inputLayout)
	{
		m_inputLayout->Release();
		m_inputLayout = nullptr;
	}
	if (m_samplerState)
	{
		m_samplerState->Release();
		m_samplerState = nullptr;
	}
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = nullptr;
	}
	return;
}

void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob* error_message, HWND hwnd, 
												WCHAR* shader_filename)
{
	char* compile_errors = nullptr;
	unsigned long buffer_size = 0;
	std::ofstream fout;

	compile_errors = (char*)(error_message->GetBufferPointer());

	buffer_size = error_message->GetBufferSize();

	fout.open("shader-error.txt");
	for (unsigned long i = 0; i < buffer_size; i++)
	{
		fout << compile_errors[i];
	}

	fout.close();

	error_message->Release();
	error_message = nullptr;

	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for more information.",
			   shader_filename, MB_OK);

	return;
}

bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* device_context, 
										   DirectX::XMMATRIX world_matrix, 
										   DirectX::XMMATRIX view_matrix, 
										   DirectX::XMMATRIX projection_matrix, 
										   ID3D11ShaderResourceView* texture, 
										   DirectX::XMFLOAT3 light_dir, 
										   DirectX::XMFLOAT4 ambient_colour, 
										   DirectX::XMFLOAT4 diffuse_colour)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	unsigned int buffer_number = 0;
	MatrixBufferType* data_ptr = nullptr;
	LightBufferType* light_dataPtr = nullptr;

	world_matrix = DirectX::XMMatrixTranspose(world_matrix);
	view_matrix = DirectX::XMMatrixTranspose(view_matrix);
	projection_matrix = DirectX::XMMatrixTranspose(projection_matrix);

	result = device_context->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD,
								 0, &mapped_resource);
	if (FAILED(result))
	{
		return false;
	}

	data_ptr = (MatrixBufferType*)mapped_resource.pData;

	data_ptr->world = world_matrix;
	data_ptr->view = view_matrix;
	data_ptr->projection = projection_matrix;

	device_context->Unmap(m_matrixBuffer, 0);

	device_context->VSSetConstantBuffers(buffer_number, 1, &m_matrixBuffer);

	device_context->PSSetShaderResources(0, 1, &texture);

	result = device_context->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD,
								 0, &mapped_resource);
	if (FAILED(result))
	{
		return false;
	}

	light_dataPtr = (LightBufferType*)mapped_resource.pData;
	light_dataPtr->ambient_colour = ambient_colour;
	light_dataPtr->diffuse_colour = diffuse_colour;
	light_dataPtr->light_direction = light_dir;
	light_dataPtr->padding = 0.0f;

	device_context->Unmap(m_lightBuffer, 0);

	buffer_number = 0;

	device_context->PSSetConstantBuffers(buffer_number, 1, &m_lightBuffer);

	return true;
}

void LightShaderClass::RenderShader(ID3D11DeviceContext* device_context, int index_count)
{
	device_context->IASetInputLayout(m_inputLayout);

	device_context->VSSetShader(m_vertexShader, nullptr, 0);
	device_context->PSSetShader(m_pixelShader, nullptr, 0);

	device_context->PSSetSamplers(0, 1, &m_samplerState);

	device_context->DrawIndexed(index_count, 0, 0);

	return;
}
