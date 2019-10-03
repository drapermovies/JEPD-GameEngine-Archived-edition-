#pragma once
#include <Windows.h>

#include "d3dclass.h"

const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass() = default;
	GraphicsClass(const GraphicsClass&) = default;
	~GraphicsClass() = default;

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
private:
	bool Render();

	D3DClass* m_D3D = nullptr;
};

