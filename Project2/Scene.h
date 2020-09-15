#pragma once
#include <d3d11.h>
#include <vector>
#include <Windows.h>

#include "cameraclass.h"
#include "d3dclass.h"
#include "LightClass.h"
#include "GameObject.h"
#include "ShaderManager.h"
#include "SceneManager.h"
#include "BitmapClass.h"
#include "TextObject.h"
#include "TimerClass.h"
#include "UserInterface.h"

class Scene
{
	friend class SceneManager;

public:
	Scene(TimerClass&, ShaderManager&);
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

	D3DClass* m_directX = nullptr;
	TimerClass& m_timer;
	CameraClass* m_camera = nullptr;

	UserInterface* m_UI = nullptr;
	ShaderManager& m_ShaderManager;

	std::vector<GameObject*> m_gameObjects;
	std::vector<LightClass*> m_lightSources;
};