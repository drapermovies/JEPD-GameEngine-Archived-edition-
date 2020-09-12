#include "FontClass.h"
#include <fstream>

#include "BufferTypes.h"

bool FontClass::Initialize(ID3D11Device* device,
						   ID3D11DeviceContext* device_context,
						   char* font_filename, 
						   WCHAR* texture_filename)
{
	bool result = LoadFontData(font_filename);
	if (result)
	{
		result = LoadTexture(device, device_context, texture_filename);
	}
	return result;
}

void FontClass::Shutdown()
{
	ReleaseTexture();
	ReleaseFontData();
	return;
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return m_texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, 
								 char* sentence, 
								 float drawX, float drawY)
{
	VertexType* vertex_ptr = (VertexType*)vertices;

	int num_letters = (int)strlen(sentence);
	int index = 0;
	for (unsigned int i = 0; i < num_letters; i++)
	{
		int letter = ((int)sentence[i]) - 32;

		//Letter is a space, so move over 3 pixels
		if (letter == 0)
		{
			drawX += 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertex_ptr[index].position = DirectX::XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertex_ptr[index].texture = DirectX::XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertex_ptr[index].position = DirectX::XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertex_ptr[index].texture = DirectX::XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertex_ptr[index].position = DirectX::XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertex_ptr[index].texture = DirectX::XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertex_ptr[index].position = DirectX::XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertex_ptr[index].texture = DirectX::XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertex_ptr[index].position = DirectX::XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertex_ptr[index].texture = DirectX::XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertex_ptr[index].position = DirectX::XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertex_ptr[index].texture = DirectX::XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
	return;
}

bool FontClass::LoadFontData(char* file_name)
{
	std::ifstream fin;
	int i = -1;
	char temp = ' ';

	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	fin.open(file_name);
	if (fin.fail()) { return false; }

	for (unsigned short i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = nullptr;
	}

	return;
}

bool FontClass::LoadTexture(ID3D11Device* device, 
							ID3D11DeviceContext* device_context,
							WCHAR* filename)
{
	bool result = false;

	m_texture = new TextureClass;

	if (m_texture)
	{
		result = m_texture->Initialize(device, device_context, (char*)filename);
	}
	return result;
}

void FontClass::ReleaseTexture()
{
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = nullptr;
	}
	return;
}
