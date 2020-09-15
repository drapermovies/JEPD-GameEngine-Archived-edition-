#pragma once
#include <Windows.h>

//Engine
#include "d3dclass.h"
#include "cameraclass.h"
#include "GameObject.h"
#include "TimerClass.h"
#include "LightClass.h"

//Shaders
#include "colorshaderclass.h"
#include "LightShaderClass.h"

//Managers
#include "ShaderManager.h"
#include "SceneManager.h"

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
	TimerClass* m_timer = nullptr;
	ShaderManager* m_ShaderManager = nullptr;

	std::unique_ptr<SceneManager> m_SceneManager = nullptr;
};

