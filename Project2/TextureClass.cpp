#include "TextureClass.h"

bool TextureClass::Initialize(ID3D11Device* device, 
							  ID3D11DeviceContext* device_context, 
							  char* file_name)
{
	bool result = false;
	int height = -1;
	int width = -1;
	D3D11_TEXTURE2D_DESC texture_desc;
	HRESULT h_result;
	unsigned int row_pitch = 0;
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;

	result = LoadTarga(file_name, height, width);
	if (!result)
	{
		return result;
	}
	
	texture_desc.Height = height;
	texture_desc.Width = width;
	texture_desc.MipLevels = 0;
	texture_desc.ArraySize = 1;
	texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.SampleDesc.Quality = 0;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texture_desc.CPUAccessFlags = 0;
	texture_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	h_result = device->CreateTexture2D(&texture_desc, nullptr, &m_texture);
	if (FAILED(h_result))
	{
		return false;
	}

	row_pitch = (width * 4) * sizeof(unsigned char);

	device_context->UpdateSubresource(m_texture, 0, nullptr, m_targaData, row_pitch, 0);

	srv_desc.Format = texture_desc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MostDetailedMip = 0;
	srv_desc.Texture2D.MipLevels = -1;

	h_result = device->CreateShaderResourceView(m_texture, &srv_desc, &m_textureView);
	if (FAILED(h_result))
	{
		return false;
	}

	device_context->GenerateMips(m_textureView);

	delete[] m_targaData;
	m_targaData = nullptr;

	return true;
}

void TextureClass::Shutdown()
{
	if (m_textureView)
	{
		m_textureView->Release();
		delete m_textureView;
	}
	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
	if (m_targaData)
	{
		delete[] m_targaData;
		m_targaData = nullptr;
	}
	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView;
}

bool TextureClass::LoadTarga(char* filename, int& height, int& width)
{
	int error = -1;
	int bpp = -1;
	int image_size = -1;
	int index = -1;
	FILE* file_ptr = nullptr;
	unsigned int count = 0;
	TargaHeader targa_file_header;
	unsigned char* targa_image = nullptr;

	error = fopen_s(&file_ptr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	count = (unsigned int)fread(&targa_file_header, sizeof(TargaHeader), 1, file_ptr);
	if (count != 1)
	{
		return false;
	}

	height = (int)targa_file_header.height;
	width = (int)targa_file_header.width;
	bpp = (int)targa_file_header.bpp;

	//Check that BPP is 32 bit and not 24 bit 
	if (bpp != 32)
	{
		return false;
	}

	//Calculate the size of the 32 bit image data
	image_size = width * height * 4;

	targa_image = new unsigned char[image_size];
	if (!targa_image)
	{
		return false;
	}

	count = (unsigned int)fread(targa_image, 1, image_size, file_ptr);
	if (count != image_size)
	{
		return false;
	}

	error = fclose(file_ptr);
	if (error != 0)
	{
		return false;
	}

	m_targaData = new unsigned char[image_size];
	if (!m_targaData)
	{
		return false;
	}

	index = 0;

	int k = (width * height * 4) - (width * 4);

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			m_targaData[index + 0] = targa_image[k + 2]; //Red
			m_targaData[index + 1] = targa_image[k + 1]; //Green
			m_targaData[index + 2] = targa_image[k + 0]; //Blue
			m_targaData[index + 3] = targa_image[k + 3]; //Alpha

			k += 4;
			index += 4;
		}
		k -= (width * 8);
	}

	delete[] targa_image;
	targa_image = nullptr;

	return true;
}
