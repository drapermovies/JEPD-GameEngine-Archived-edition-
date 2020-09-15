#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include "BufferTypes.h"

class ColourShaderClass
{
public: 
	ColourShaderClass() = default;
	ColourShaderClass(const ColourShaderClass&) = default;
	~ColourShaderClass() = default;

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX, DirectX::XMMATRIX, 
																DirectX::XMMATRIX);
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX,
																		DirectX::XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D11Buffer* m_matrixBuffer = nullptr;
};