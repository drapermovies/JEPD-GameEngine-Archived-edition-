#include "TextObject.h"

bool TextObject::Initialize(ID3D11Device* device,
	ID3D11DeviceContext* device_context,
	int screen_width, int screen_height,
	int max_length, FontClass* font, char* text,
	int pos_x, int pos_y, float red, float green, float blue)
{
	m_ScreenWidth = screen_width;
	m_ScreenHeight = screen_height;

	m_MaxLength = max_length;

	bool result = InitializeSentence(device, device_context, 
									 font, text, 
									 pos_x, pos_y, 
									 red, green, blue);

	return result;
}

void TextObject::Shutdown()
{
	ReleaseSentence(&sentence);

	return;
}

bool TextObject::Render(ID3D11DeviceContext* device_context, 
						ShaderManager* shader_manager,
						DirectX::XMMATRIX world_matrix, 
					    DirectX::XMMATRIX view_matrix,
						DirectX::XMMATRIX ortho_matrix,
						ID3D11ShaderResourceView* font_texture)
{
	bool result = RenderSentence(device_context, shader_manager, world_matrix, 
								 view_matrix, ortho_matrix, font_texture);

	return result;
}

bool TextObject::InitializeSentence(ID3D11Device* device, ID3D11DeviceContext* device_context,
									FontClass* font, char* text,
									int pos_x, int pos_y,
									float red, float green, float blue)
{
	HRESULT result;
	D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;
	D3D11_SUBRESOURCE_DATA vertex_data, index_data;

	sentence = new SentenceType;
	sentence->vertex_count = 6 * m_MaxLength;
	sentence->index_count = 6 * m_MaxLength;

	sentence->max_length = m_MaxLength;
	m_MaxLength = -1;

	VertexType* vertices = new VertexType[sentence->vertex_count];
	if (!vertices)
	{
		return false;
	}

	unsigned long* indices = new unsigned long[sentence->index_count];
	if (!indices)
	{
		return false;
	}

	memset(vertices, 0, sizeof(VertexType) * sentence->vertex_count);

	for (int i = 0; i < sentence->index_count; i++)
	{
		indices[i] = i;
	}

	vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	vertex_buffer_desc.ByteWidth = sizeof(VertexType) * sentence->vertex_count;
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertex_buffer_desc.MiscFlags = 0;
	vertex_buffer_desc.StructureByteStride = 0;

	vertex_data.pSysMem = vertices;
	vertex_data.SysMemPitch = 0;
	vertex_data.SysMemSlicePitch = 0;
	
	result = device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &sentence->vertex_buffer);
	if (FAILED(result))
	{
		return false;
	}

	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_desc.ByteWidth = sizeof(unsigned long) * sentence->index_count;
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.CPUAccessFlags = 0;
	index_buffer_desc.MiscFlags = 0;
	index_buffer_desc.StructureByteStride = 0;

	index_data.pSysMem = indices;
	index_data.SysMemPitch = 0;
	index_data.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&index_buffer_desc, &index_data, &sentence->index_buffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return UpdateSentence(device_context, font, text, pos_x, pos_y, red, green, blue);
}

bool TextObject::UpdateSentence(ID3D11DeviceContext* device_context,
							    FontClass* font, char* in_text,
								int pos_x, int pos_y,
								float r, float g, float b)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mapped_resource;

	sentence->red = r;
	sentence->green = g;
	sentence->blue = b;

	int num_letters = (int)strlen(in_text);

	if (num_letters > sentence->max_length)
	{
		return false;
	}

	VertexType* vertices = new VertexType[sentence->vertex_count];
	if (!vertices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * sentence->vertex_count));

	float draw_x = (float)(((m_ScreenWidth / 2) * -1) + pos_x);
	float draw_y = (float)((m_ScreenHeight / 2) - pos_y);

	font->BuildVertexArray((void*)vertices, in_text, draw_x, draw_y);

	result = device_context->Map(sentence->vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
								 &mapped_resource);

	if (FAILED(result))
	{
		return false;
	}

	void* vertices_ptr = (void*)mapped_resource.pData;

	memcpy(vertices_ptr, (void*)vertices, (sizeof(VertexType) * sentence->vertex_count));

	device_context->Unmap(sentence->vertex_buffer, 0);

	delete[] vertices;
	vertices = nullptr;

	return true;
}

void TextObject::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		if ((*sentence)->vertex_buffer)
		{
			(*sentence)->vertex_buffer->Release();
			(*sentence)->vertex_buffer = nullptr;
		}
		if ((*sentence)->index_buffer)
		{
			(*sentence)->index_buffer->Release();
			(*sentence)->index_buffer = nullptr;
		}

		delete* sentence;
		*sentence = nullptr;
	}
	return;
}

bool TextObject::RenderSentence(ID3D11DeviceContext* device_context, 
								ShaderManager* shader_manager,
								DirectX::XMMATRIX world_matrix, 
								DirectX::XMMATRIX view_matrix,
								DirectX::XMMATRIX ortho_matrix,
								ID3D11ShaderResourceView* font_texture)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	device_context->IASetVertexBuffers(0, 1, &sentence->vertex_buffer, &stride, &offset);
	device_context->IASetIndexBuffer(sentence->index_buffer, DXGI_FORMAT_R32_UINT, 0);
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DirectX::XMFLOAT4 pixel_colour = DirectX::XMFLOAT4(sentence->red,
													sentence->green,
													sentence->blue, 1.0f);

	bool result = shader_manager->RenderFontShader(device_context, sentence->index_count, 
												   world_matrix, view_matrix, ortho_matrix, 
												   font_texture, pixel_colour);

	return result;
}
