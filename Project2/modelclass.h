#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class ModelClass
{
public:
	ModelClass() = default;
	ModelClass(const ModelClass&) = default;
	~ModelClass() = default;

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	void ModifyVertex(VertexType& vertex, DirectX::XMFLOAT3, DirectX::XMFLOAT4);

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

	int m_vertex_count = -1;
	int m_index_count = -1;
};