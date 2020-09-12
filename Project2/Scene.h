#pragma once
#include <d3d11.h>
#include <vector>

#include "cameraclass.h"
#include "d3dclass.h"
#include "LightClass.h"
#include "LightShaderClass.h"
#include "TextureShaderClass.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "BitmapClass.h"
#include "TimerClass.h"

class Scene
{
	friend class SceneManager;

public:
	Scene(TimerClass&, LightShaderClass&, TextureShaderClass&);
	Scene(const Scene&) = default;
	~Scene() = default;

	virtual bool Initialize(float, float) = 0;
	virtual void Shutdown() = 0;

	virtual void Update() = 0;
	
	virtual bool Render();

	void SetName(std::string);
	std::string GetName() { return name; }

	void SetActive(bool);
	bool GetActive() { return is_active; }

	CameraClass* GetCamera() { return m_camera; }

	std::vector<LightClass*> GetLightSources() { return m_lightSources; }
protected: 
	std::string name = "Scene";
	bool is_active = true;

	D3DClass* m_directX;
	TimerClass& m_timer;
	LightShaderClass& m_LightShader;
	CameraClass* m_camera;

	TextureShaderClass& m_CanvasShader;

	std::vector<GameObject*> m_gameObjects;
	std::vector<LightClass*> m_lightSources;
	std::vector<BitmapClass*> m_canvasObjects;
};