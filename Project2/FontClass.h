#pragma once
#include <d3d11.h>
#include "TextureClass.h"

class FontClass
{
public:
	FontClass() = default;
	FontClass(const FontClass&) = default;
	~FontClass() = default;

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, char*, float, float);
private: 
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
	void ReleaseTexture();

	struct FontType
	{
		float left, right;
		int size;
	};

	FontType* m_Font = nullptr;
	TextureClass* m_texture = nullptr;
};