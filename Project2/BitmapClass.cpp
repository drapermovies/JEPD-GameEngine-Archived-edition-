#include "BitmapClass.h"

bool BitmapClass::Initialize(ID3D11Device* device, 
							 ID3D11DeviceContext* device_context,
							 int screen_width, 
							 int screen_height,
							 WCHAR* texture_fileName, 
							 int bitmap_width, 
							 int bitmap_height)
{
	bool result = false;

	m_screenWidth = screen_width;
	m_screenHeight = screen_height;

	m_bitmapWidth = bitmap_width;
	m_bitmapHeight = bitmap_height;

	result = InitializeBuffers(device);

	if (result)
	{
		result = LoadTexture(device, device_context, texture_fileName);
	}

	return result;
}

void BitmapClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
	return;
}

bool BitmapClass::Render(ID3D11DeviceContext* device, int posX, int posY)
{
	bool result = false;
	result = UpdateBuffers(device, posX, posY);
	if (result)
	{
		RenderBuffers(device);
	}
	return result;
}

int BitmapClass::GetIndexCount() const
{
	return m_indexCount;
}

ID3D11ShaderResourceView* BitmapClass::GetTexture() const
{
	return m_texture->GetTexture();
}

bool BitmapClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices = nullptr;
	unsigned long* indices = nullptr;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertex_data, index_data;
	HRESULT result; 
	int i = -1;

	m_vertexCount = 6;
	m_indexCount = m_vertexCount;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	for (unsigned int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertex_data.pSysMem = vertices;
	vertex_data.SysMemPitch = 0;
	vertex_data.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertex_data, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	index_data.pSysMem = indices;
	index_data.SysMemPitch = 0;
	index_data.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &index_data, &m_indexBuffer);
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

void BitmapClass::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	return;
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* device_context, int posX, int posY)
{
	VertexType* vertices = nullptr;
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	VertexType* vertices_ptr = nullptr;
	HRESULT result;

	if ((posX == m_previousPosX) && (posY == m_previousPosY))
	{
		//Nothing has changed, so nothing is updated
		return true;
	}

	m_previousPosX = posX;
	m_previousPosY = posY;

	float left = (float)((m_screenWidth / 2) * -1) + (float)posX;
	float right = left + (float)m_bitmapWidth;

	float top = (float)(m_screenHeight / 2) - (float)posY;
	float bottom = top - (float)m_bitmapHeight;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	vertices[0].position = DirectX::XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].texture = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = DirectX::XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = DirectX::XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = DirectX::XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = DirectX::XMFLOAT2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = DirectX::XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].texture = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = DirectX::XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].texture = DirectX::XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = DirectX::XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = DirectX::XMFLOAT2(1.0f, 1.0f);

	result = device_context->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
	{
		return false;
	}

	vertices_ptr = (VertexType*)mapped_resource.pData;

	memcpy(vertices_ptr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	device_context->Unmap(m_vertexBuffer, 0);

	delete[] vertices;
	vertices = nullptr;

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* device_context)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	device_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	device_context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool BitmapClass::LoadTexture(ID3D11Device* device, 
							  ID3D11DeviceContext* device_context,
							  WCHAR* file_name)
{
	bool result = false;

	m_texture = new TextureClass;
	if (!m_texture)
	{
		return result;
	}

	result = m_texture->Initialize(device, device_context, (char*)file_name);

	return result;
}

void BitmapClass::ReleaseTexture()
{
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = nullptr;
	}
	return;
}
