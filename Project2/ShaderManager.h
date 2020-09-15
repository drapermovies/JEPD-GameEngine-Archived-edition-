#pragma once
#include "colorshaderclass.h"
#include "FontShaderClass.h"
#include "LightShaderClass.h"
#include "TextureShaderClass.h"

class ShaderManager
{
public:
	ShaderManager() = default;
	ShaderManager(const ShaderManager&) = default;
	~ShaderManager() = default;

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool RenderFontShader(ID3D11DeviceContext*, int,
							DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX,
							ID3D11ShaderResourceView*, DirectX::XMFLOAT4);

	bool RenderLightShader(ID3D11DeviceContext*, int,
							DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX,
							ID3D11ShaderResourceView*, DirectX::XMFLOAT3, DirectX::XMFLOAT4);
private:
	FontShader* m_FontShader = nullptr;
	LightShaderClass* m_LightShader = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
};