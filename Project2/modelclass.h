#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "TextureClass.h"
#include <fstream>

class ModelClass
{
	friend class GameObject;

public:
	ModelClass() = default;
	ModelClass(const ModelClass&) = default;
	~ModelClass() = default;

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
protected:
	DirectX::XMFLOAT3 GetPosition() { return m_position; }
	void SetPosition(float, float, float);

	DirectX::XMFLOAT3 GetScale() { return m_scale; }
	void SetScale(float, float, float);

	DirectX::XMFLOAT3 GetRotation() { return m_rotation; }
	void SetRotation(float, float, float);
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

	DirectX::XMFLOAT3 m_position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 m_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT3 m_rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool InitializeIndexBuffer(ID3D11Device*, unsigned long*);
	bool InitializeVertexBuffer(ID3D11Device*, VertexType*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	void ModifyVertex(VertexType& vertex, DirectX::XMFLOAT3, DirectX::XMFLOAT2, DirectX::XMFLOAT3);

	bool LoadModel(char*);
	void ReleaseModel();

	bool GenerateVertices(ID3D11DeviceContext*);

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

	ID3D11DeviceContext* m_deviceContext = nullptr;

	int m_vertex_count = -1;
	int m_index_count = -1;

	TextureClass* m_texture = nullptr;
	ModelType* m_model = nullptr;

	char* file_path = nullptr;
};