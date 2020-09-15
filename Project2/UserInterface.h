#pragma once
#include "d3dclass.h"
#include "TextObject.h"
#include "BitmapClass.h"
#include "ShaderManager.h"

class UserInterface
{
public:
	UserInterface() = default;
	UserInterface(const UserInterface&) = default;
	~UserInterface() = default;

	bool Initialize(D3DClass*, int, int);
	void Shutdown();

	bool Frame(ID3D11DeviceContext*, int, float, float, float, float, float, float);
	bool Render(D3DClass*, ShaderManager*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);

private:
	FontClass* m_Font = nullptr;
	TextObject* m_Text = nullptr;
};