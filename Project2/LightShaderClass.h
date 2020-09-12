#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include "MatrixBufferType.h"

class LightShaderClass
{
public:
	LightShaderClass() = default;
	LightShaderClass(const LightShaderClass&) = default;
	~LightShaderClass() = default;

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX, DirectX::XMMATRIX,
				DirectX::XMMATRIX, ID3D11ShaderResourceView*, DirectX::XMFLOAT3,
				DirectX::XMFLOAT4, DirectX::XMFLOAT4, DirectX::XMFLOAT3, 
				DirectX::XMFLOAT4, float);
private:
	struct CameraBufferType
	{
		DirectX::XMFLOAT3 camera_position;
		float padding;
	};

	struct LightBufferType
	{
		DirectX::XMFLOAT4 ambient_colour;
		DirectX::XMFLOAT4 diffuse_colour;
		DirectX::XMFLOAT3 light_direction;
		float specularPower;
		DirectX::XMFLOAT4 specular_colour;
	};

	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX,
							DirectX::XMMATRIX, DirectX::XMMATRIX,
							ID3D11ShaderResourceView*, DirectX::XMFLOAT3,
							DirectX::XMFLOAT4, DirectX::XMFLOAT4, DirectX::XMFLOAT3,
							DirectX::XMFLOAT4, float);
	void RenderShader(ID3D11DeviceContext*, int);

	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;
	ID3D11SamplerState* m_samplerState = nullptr;
	ID3D11Buffer* m_matrixBuffer = nullptr;

	ID3D11Buffer* m_cameraBuffer = nullptr;
	ID3D11Buffer* m_lightBuffer = nullptr;
};