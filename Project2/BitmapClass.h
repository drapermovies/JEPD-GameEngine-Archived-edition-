#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "TextureClass.h"

class BitmapClass
{
public:
	BitmapClass() = default;
	BitmapClass(const BitmapClass&) = default;
	~BitmapClass() = default;

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int, WCHAR*, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount() const;
	ID3D11ShaderResourceView* GetTexture() const;
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
	void ReleaseTexture();

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	TextureClass* m_texture = nullptr;

	int m_screenWidth = -1;
	int m_screenHeight = -1;
	int m_bitmapWidth = -1;
	int m_bitmapHeight = -1;
	int m_previousPosX = -1;
	int m_previousPosY = -1;
};