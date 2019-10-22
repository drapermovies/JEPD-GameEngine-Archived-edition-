#pragma once
#include <Windows.h>

#include "d3dclass.h"
#include "cameraclass.h"
#include "GameObject.h"
#include "colorshaderclass.h"
#include "LightShaderClass.h"
#include "LightClass.h"
#include "Customer.h"

const bool FULL_SCREEN = false;
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
	bool Render(float);

	D3DClass* m_D3D = nullptr;
	CameraClass* m_camera = nullptr;
	GameObject* m_object = nullptr;
	LightShaderClass* m_lightShader = nullptr;
	LightClass* m_light = nullptr;
	Customer* customer = nullptr;
};

