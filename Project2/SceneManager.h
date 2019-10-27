#pragma once
#include <vector>

#include "Scene.h"

class SceneManager
{
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
private:
	std::vector<Scene*> scenes;
};