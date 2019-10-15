#include "modelclass.h"

bool ModelClass::Initialize(ID3D11Device* device)
{
	bool result = false;

	result = InitializeBuffers(device);

	return result;
}

void ModelClass::Shutdown()
{
	ShutdownBuffers();
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

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices = nullptr;
	unsigned long* indices = nullptr;
	CD3D11_BUFFER_DESC vertex_buffer_desc;
	CD3D11_BUFFER_DESC index_buffer_desc;
	D3D11_SUBRESOURCE_DATA vertex_data;
	D3D11_SUBRESOURCE_DATA index_data;
	HRESULT result;

	m_vertex_count = 3;

	m_index_count = 3;

	vertices = new VertexType[m_vertex_count];
	if (!vertices) { return false; }

	indices = new unsigned long[m_index_count];
	if (!indices)
	{
		return false;
	}

	ModifyVertex(vertices[0], DirectX::XMFLOAT3(-1.0f, -1, 0.0f), 
						DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	ModifyVertex(vertices[1], DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
						DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	ModifyVertex(vertices[2], DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f),
						DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	//Vertex Buffer

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

	//Index Buffer

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

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

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
	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* device_context)
{
	unsigned int stride = 0;
	unsigned int offset = 0;

	stride = sizeof(VertexType);
	offset = 0;

	device_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	device_context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void ModelClass::ModifyVertex(VertexType& vertex, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color)
{
	vertex.position = pos;
	vertex.color = color;
}
