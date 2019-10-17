#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

class TextureShaderClass
{
public:
	TextureShaderClass() = default;
	TextureShaderClass(const TextureShaderClass&) = default;
	~TextureShaderClass() = default;

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX, DirectX::XMMATRIX, 
								DirectX::XMMATRIX, ID3D11ShaderResourceView*);
private:
	struct MatrixBufferType 
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameter(ID3D11DeviceContext*, DirectX::XMMATRIX, 
								DirectX::XMMATRIX, DirectX::XMMATRIX, 
											ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D11Buffer* m_matrixBuffer = nullptr;

	ID3D11SamplerState* m_sampleState = nullptr;
};