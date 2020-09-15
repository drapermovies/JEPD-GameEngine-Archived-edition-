#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "BufferTypes.h"

class FontShader
{
public:
	FontShader() = default;
	FontShader(const FontShader&) = default;
	~FontShader() = default;

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int,
				DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX,
				ID3D11ShaderResourceView*, DirectX::XMFLOAT4);

private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D11Buffer* m_ConstBuffer = nullptr;
	ID3D11SamplerState* m_SampleState = nullptr;
	ID3D11Buffer* m_PixelBuffer = nullptr;

	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*,
							DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX,
							ID3D11ShaderResourceView*, DirectX::XMFLOAT4);

	void RenderShader(ID3D11DeviceContext*, int);
};