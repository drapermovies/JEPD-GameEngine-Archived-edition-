#include "TextureShaderClass.h"

bool TextureShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result = false;
	result = InitializeShader(device, hwnd, (WCHAR*)L"../x64/Debug/Texture.vs", 
											(WCHAR*)L"../x64/Debug/Texture.ps");
	return result;
}

void TextureShaderClass::Shutdown()
{
	ShutdownShader();
	return;
}

bool TextureShaderClass::Render(ID3D11DeviceContext* device_context, int index_count, 
								DirectX::XMMATRIX world_matrix, DirectX::XMMATRIX view_matrix, 
								DirectX::XMMATRIX projection_matrix, ID3D11ShaderResourceView* texture)
{
	bool result = false;

	result = SetShaderParameter(device_context, world_matrix, view_matrix, projection_matrix, texture);
	if (result)
	{
		RenderShader(device_context, index_count);
	}

	return result;
}

bool TextureShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd,
										WCHAR* vs_filename , WCHAR* ps_filename)
{
	HRESULT result = S_OK;
	ID3D10Blob* error_message = nullptr;
	ID3D10Blob* vertex_shader_buffer = nullptr;
	ID3D10Blob* pixel_shader_buffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygon_layout[2];
	unsigned int num_elements = 0;
	D3D11_BUFFER_DESC matrix_buffer_desc;

	D3D11_SAMPLER_DESC sampler_desc;

	result = D3DCompileFromFile(vs_filename, nullptr, nullptr, "TextureVertexShader",
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
			MessageBox(hwnd, vs_filename, L"Missing Shader File", MB_OK);
		}
		return false;
	}
	
	result = D3DCompileFromFile(ps_filename, nullptr, nullptr, "TexturePixelShader",
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
			MessageBox(hwnd, ps_filename, L"Missing Shader File", MB_OK);
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

	num_elements = sizeof(polygon_layout) / sizeof(polygon_layout[0]);

	result = device->CreateInputLayout(polygon_layout, num_elements,
							vertex_shader_buffer->GetBufferPointer(),
							vertex_shader_buffer->GetBufferSize(),
															&m_layout);
	if (FAILED(result))
	{
		return false;
	}

	vertex_shader_buffer->Release();
	vertex_shader_buffer = nullptr;

	pixel_shader_buffer->Release();
	pixel_shader_buffer = nullptr;

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

	result = device->CreateSamplerState(&sampler_desc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	return	true;
}

void TextureShaderClass::ShutdownShader()
{
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = nullptr;
	}
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}
	if (m_layout)
	{
		m_layout->Release();
		m_layout = nullptr;
	}
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}
	return;
}

void TextureShaderClass::OutputShaderErrorMessage(ID3D10Blob* error_message, HWND hwnd, 
																WCHAR* shader_filename)
{
	char* compile_errors = nullptr;
	unsigned long long buffer_size = 0;
	std::ofstream fout;

	compile_errors = (char*)(error_message->GetBufferPointer());

	buffer_size = error_message->GetBufferSize();

	fout.open("shader-error.txt");

	for (unsigned long long i = 0; i < buffer_size; i++)
	{
		fout << compile_errors[i];
	}

	fout.close();

	error_message->Release();
	error_message = nullptr;

	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for more information.", shader_filename, MB_OK);

	return;
}

bool TextureShaderClass::SetShaderParameter(ID3D11DeviceContext* device_context, 
												DirectX::XMMATRIX world_matrix, 
												DirectX::XMMATRIX view_matrix, 
												DirectX::XMMATRIX projection_matrix, 
												ID3D11ShaderResourceView* texture)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	MatrixBufferType* data_ptr = nullptr;
	unsigned int buffer_number = 0;

	world_matrix = DirectX::XMMatrixTranspose(world_matrix);
	view_matrix = DirectX::XMMatrixTranspose(view_matrix);
	projection_matrix = DirectX::XMMatrixTranspose(projection_matrix);

	result = device_context->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
															&mapped_resource);
	if (FAILED(result))
	{
		return false;
	}

	data_ptr = (MatrixBufferType*)mapped_resource.pData;

	data_ptr->world = world_matrix;
	data_ptr->view = view_matrix;
	data_ptr->projection = projection_matrix;

	device_context->Unmap(m_matrixBuffer, 0);

	buffer_number = 0;

	device_context->VSSetConstantBuffers(buffer_number, 1, &m_matrixBuffer);

	device_context->PSSetShaderResources(0, 1, &texture);

	return true;
}

void TextureShaderClass::RenderShader(ID3D11DeviceContext* device_context, int index_count)
{
	device_context->IASetInputLayout(m_layout);

	device_context->VSSetShader(m_vertexShader, nullptr, 0);
	device_context->PSSetShader(m_pixelShader, nullptr, 0);

	device_context->PSSetSamplers(0, 1, &m_sampleState);

	device_context->DrawIndexed(index_count, 0, 0);

	return;
}
