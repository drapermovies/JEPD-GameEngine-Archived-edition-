#pragma once
#include <vector>

#include "d3dclass.h"
#include "Scene.h"
#include "TimerClass.h"

class SceneManager
{
	friend class Scene;
public:
	SceneManager() = default;
	SceneManager(const SceneManager&) = default;
	~SceneManager() = default;

	void Shutdown();

	void Update();
	bool Render();

	std::vector<Scene*> GetActiveScenes();

	void AddScene(Scene*);
	Scene* GetScene(std::string);

	CameraClass* GetSceneCamera();

	D3DClass* directX;
private:
	std::vector<Scene*> scenes;
	std::unique_ptr<TimerClass> m_timer;
};