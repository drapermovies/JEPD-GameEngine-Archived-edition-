#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "TextureClass.h"

class ModelClass
{
public:
	ModelClass() = default;
	ModelClass(const ModelClass&) = default;
	~ModelClass() = default;

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 color;
	};

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	void ModifyVertex(VertexType& vertex, DirectX::XMFLOAT3, DirectX::XMFLOAT2, DirectX::XMFLOAT3);

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

	int m_vertex_count = -1;
	int m_index_count = -1;

	TextureClass* m_texture = nullptr;
};