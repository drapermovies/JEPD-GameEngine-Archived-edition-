#include "FontShaderClass.h"
#include <fstream>

bool FontShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result = InitializeShader(device, hwnd,
								   (WCHAR*)L"../x64/Debug/data/Shaders/font.vs",
								   (WCHAR*)L"../x64/Debug/data/Shaders/font.ps");
	return result;
}

void FontShader::Shutdown()
{
	ShutdownShader();
	return;
}

bool FontShader::Render(ID3D11DeviceContext* device_context, 
						int index_count, 
						DirectX::XMMATRIX world_matrix, 
						DirectX::XMMATRIX view_matrix, 
					    DirectX::XMMATRIX projection_matrix, 
						ID3D11ShaderResourceView* texture, 
						DirectX::XMFLOAT4 pixel_colour)
{
	bool result = SetShaderParameters(device_context, 
									  world_matrix, view_matrix, projection_matrix, 
									  texture, pixel_colour);

	if (result)
	{
		RenderShader(device_context, index_count);
	}
	return result;
}

bool FontShader::InitializeShader(ID3D11Device* device, HWND hwnd,
	WCHAR* vs_filename, WCHAR* ps_filename)
{
	HRESULT result;
	ID3D10Blob* error_message = nullptr;
	ID3D10Blob* vertex_shader_buffer = nullptr;
	ID3D10Blob* pixel_shader_buffer = nullptr;

	D3D11_INPUT_ELEMENT_DESC polygon_layout[2];
	unsigned int num_elements = 0;
	D3D11_BUFFER_DESC constant_buffer_desc;
	D3D11_SAMPLER_DESC sampler_desc;
	D3D11_BUFFER_DESC pixel_buffer_desc;

	result = D3DCompileFromFile(vs_filename, nullptr, nullptr,
		"FontVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertex_shader_buffer, &error_message);
	if (FAILED(result))
	{
		if (error_message)
		{
			OutputShaderErrorMessage(error_message, hwnd, vs_filename);
		}
		else
		{
			MessageBox(hwnd, vs_filename, L"Missing Shader file", MB_OK);
		}
		return false;
	}

	result = D3DCompileFromFile(ps_filename, nullptr, nullptr,
		"FontPixelShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixel_shader_buffer, &error_message);
	if (FAILED(result))
	{
		if (error_message)
		{
			OutputShaderErrorMessage(error_message, hwnd, ps_filename);
		}
		else
		{
			MessageBox(hwnd, ps_filename, L"Missing Shader file", MB_OK);
		}
		return false;
	}

	result = device->CreateVertexShader(vertex_shader_buffer->GetBufferPointer(),
		vertex_shader_buffer->GetBufferSize(),
		nullptr, &m_VertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(pixel_shader_buffer->GetBufferPointer(),
		pixel_shader_buffer->GetBufferSize(),
		nullptr, &m_PixelShader);
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
		vertex_shader_buffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	vertex_shader_buffer->Release();
	vertex_shader_buffer = nullptr;

	pixel_shader_buffer->Release();
	pixel_shader_buffer = nullptr;

	constant_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	constant_buffer_desc.ByteWidth = sizeof(ConstBufferType);
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constant_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constant_buffer_desc.MiscFlags = 0;
	constant_buffer_desc.StructureByteStride = 0;

	result = device->CreateBuffer(&constant_buffer_desc, nullptr, &m_ConstBuffer);
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

	result = device->CreateSamplerState(&sampler_desc, &m_SampleState);
	if (FAILED(result))
	{
		return false;
	}

	pixel_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	pixel_buffer_desc.ByteWidth = sizeof(PixelBufferType);
	pixel_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pixel_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pixel_buffer_desc.MiscFlags = 0;
	pixel_buffer_desc.StructureByteStride = 0;

	result = device->CreateBuffer(&pixel_buffer_desc, nullptr, &m_PixelBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void FontShader::ShutdownShader()
{
	if (m_PixelBuffer)
	{
		m_PixelBuffer->Release();
		m_PixelBuffer = nullptr;
	}
	if (m_SampleState)
	{
		m_SampleState->Release();
		m_SampleState = nullptr;
	}
	if (m_ConstBuffer)
	{
		m_ConstBuffer->Release();
		m_ConstBuffer = nullptr;
	}
	if (m_layout)
	{
		m_layout->Release();
		m_layout = nullptr;
	}
	if (m_PixelShader)
	{
		m_PixelShader->Release();
		m_PixelShader = nullptr;
	}
	if (m_VertexShader)
	{
		m_VertexShader->Release();
		m_VertexShader = nullptr;
	}
	return;
}

void FontShader::OutputShaderErrorMessage(ID3D10Blob* error_message, HWND hwnd, 
										  WCHAR* shader_filename)
{
	std::ofstream fout;

	char* compile_errors = (char*)error_message->GetBufferPointer();
	unsigned long buffer_size = error_message->GetBufferSize();
	
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

bool FontShader::SetShaderParameters(ID3D11DeviceContext* device_context, 
									 DirectX::XMMATRIX world_matrix, 
									 DirectX::XMMATRIX view_matrix, 
									 DirectX::XMMATRIX projection_matrix, 
									 ID3D11ShaderResourceView* texture, 
									 DirectX::XMFLOAT4 pixel_colour)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mapped_resource;

	//Prepare matrices for shader
	world_matrix = DirectX::XMMatrixTranspose(world_matrix);
	view_matrix = DirectX::XMMatrixTranspose(view_matrix);
	projection_matrix = DirectX::XMMatrixTranspose(projection_matrix);
	
	result = device_context->Map(m_ConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
	{
		return false;
	}

	ConstBufferType* data_ptr = (ConstBufferType*)mapped_resource.pData;

	data_ptr->world = world_matrix;
	data_ptr->view = view_matrix;
	data_ptr->projection = projection_matrix;

	device_context->Unmap(m_ConstBuffer, 0);

	unsigned int buffer_number = 0;

	device_context->VSSetConstantBuffers(buffer_number, 1, &m_ConstBuffer);
	device_context->PSSetShaderResources(0, 1, &texture);

	result = device_context->Map(m_PixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
	{
		return false;
	}

	PixelBufferType* pixel_data = (PixelBufferType*)mapped_resource.pData;
	pixel_data->pixel_colour = pixel_colour;

	device_context->Unmap(m_PixelBuffer, 0);

	buffer_number = 0;

	device_context->PSSetConstantBuffers(buffer_number, 1, &m_PixelBuffer);

	return true;
}

void FontShader::RenderShader(ID3D11DeviceContext* device_context, int index_count)
{
	device_context->IASetInputLayout(m_layout);

	device_context->VSSetShader(m_VertexShader, nullptr, 0);
	device_context->PSSetShader(m_PixelShader, nullptr, 0);

	device_context->PSSetSamplers(0, 1, &m_SampleState);

	device_context->DrawIndexed(index_count, 0, 0);

	return;
}
