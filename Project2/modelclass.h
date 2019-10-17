#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "TextureClass.h"
#include <fstream>

class ModelClass
{
public:
	ModelClass() = default;
	ModelClass(const ModelClass&) = default;
	~ModelClass() = default;

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x = 0;
		float y = 0;
		float z = 0;

		float tu = 0;
		float tv = 0;

		float nx = 0;
		float ny = 0;
		float nz = 0;
	};

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	void ModifyVertex(VertexType& vertex, DirectX::XMFLOAT3, DirectX::XMFLOAT2, DirectX::XMFLOAT3);

	bool LoadModel(char*);
	void ReleaseModel();

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

	int m_vertex_count = -1;
	int m_index_count = -1;

	TextureClass* m_texture = nullptr;
	ModelType* m_model = nullptr;
};