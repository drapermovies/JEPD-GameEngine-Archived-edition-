#include "modelclass.h"
#include <iostream>

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* device_context,
							char* model_filename, WCHAR* texture_filename)
{
	bool result = false;

	m_deviceContext = device_context;

	result = LoadModel(model_filename);
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(device);

	if (result)
	{
		result = LoadTexture(device, device_context, (char*)texture_filename);
	}

	return result;
}

void ModelClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
	ReleaseModel();
	return;
}

void ModelClass::Render(ID3D11DeviceContext* device_context)
{
	RenderBuffers(device_context);

	return;
}

int ModelClass::GetIndexCount()
{
	return m_index_count;
}

void ModelClass::SetPosition(float x, float y, float z)
{
	m_position = DirectX::XMFLOAT3(x, y, z);
	GenerateVertices();
}

void ModelClass::SetScale(float x, float y, float z)
{
	m_scale = DirectX::XMFLOAT3(x, y, z);
	GenerateVertices();
}

void ModelClass::SetRotation(float x, float y, float z)
{
	m_rotation = DirectX::XMFLOAT3(x, y, z);
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	HRESULT result;

	indices = new unsigned long[m_index_count];
	if (!indices)
	{
		return false;
	}

	vertices = new VertexType[m_vertex_count];
	if (!vertices)
	{
		return false;
	}

	GenerateVertices();

	InitializeVertexBuffer(device);

	InitializeIndexBuffer(device);

	return true;
}

void ModelClass::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		delete m_indexBuffer;
		m_indexBuffer = nullptr;
	}

	if (m_vertexBuffer)
	{
		delete m_vertexBuffer;
		m_vertexBuffer = nullptr;
	}
	if (file_path)
	{
		delete file_path;
		file_path = nullptr;
	}
	if (vertices)
	{
		delete vertices;
		vertices = nullptr;
	}
	if (indices)
	{
		delete indices;
		indices = nullptr;
	}
	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* device_context)
{
	unsigned int stride = 0;
	unsigned int offset = 0;

	stride = sizeof(VertexType);

	device_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	device_context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::InitializeIndexBuffer(ID3D11Device* device)
{
	HRESULT result = S_OK;
	CD3D11_BUFFER_DESC index_buffer_desc;
	D3D11_SUBRESOURCE_DATA index_data;

	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_desc.ByteWidth = sizeof(unsigned long) * m_index_count;
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.CPUAccessFlags = 0;
	index_buffer_desc.MiscFlags = 0;
	index_buffer_desc.StructureByteStride = 0;

	index_data.pSysMem = indices;
	index_data.SysMemPitch = 0;
	index_data.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&index_buffer_desc, &index_data, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return false;
}

bool ModelClass::InitializeVertexBuffer(ID3D11Device* device)
{
	CD3D11_BUFFER_DESC vertex_buffer_desc;
	D3D11_SUBRESOURCE_DATA vertex_data;
	HRESULT result = S_OK;

	vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_buffer_desc.ByteWidth = sizeof(VertexType) * m_vertex_count;
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.CPUAccessFlags = 0;
	vertex_buffer_desc.MiscFlags = 0;
	vertex_buffer_desc.StructureByteStride = 0;

	vertex_data.pSysMem = vertices;
	vertex_data.SysMemPitch = 0;
	vertex_data.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return false;
}

bool ModelClass::LoadTexture(ID3D11Device* device, 
							 ID3D11DeviceContext* device_context, 
							 char* file_name)
{
	bool result = false;

	m_texture = new TextureClass;
	if (!m_texture) { return false; }

	result = m_texture->Initialize(device, device_context, file_name);

	return result;
}

void ModelClass::ReleaseTexture()
{
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = nullptr;
	}
	return;
}

void ModelClass::ModifyVertex(VertexType& vertex, 
							  DirectX::XMFLOAT3 pos, 
							  DirectX::XMFLOAT2 tex, 
							  DirectX::XMFLOAT3 normal)
{
	vertex.position = pos;
	vertex.texture = tex;
	vertex.normal = normal;
}

bool ModelClass::LoadModel(char* filename)
{
	file_path = filename;

	std::fstream fin;
	char input = ' ';

	fin.open(filename);

	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_vertex_count;

	m_index_count = m_vertex_count;

	m_model = new ModelType[m_vertex_count];
	if (!m_model)
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	for (int i = 0; i < m_vertex_count; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}
	fin.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = nullptr;
	}
	return;
}

bool ModelClass::GenerateVertices()
{
	float x = 0, y = 0, z = 0;
	ModelType* model = nullptr;

	for (unsigned int i = 0; i < m_vertex_count; i++)
	{
		x = m_model[i].x;
		y = m_model[i].y;
		z = m_model[i].z;

		ModifyVertex(vertices[i],
			DirectX::XMFLOAT3(x, y, z),
			DirectX::XMFLOAT2(m_model[i].tu, m_model[i].tv),
			DirectX::XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz));
		indices[i] = i;
	}

	return true;
}
