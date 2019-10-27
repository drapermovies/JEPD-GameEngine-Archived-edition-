#include "SceneManager.h"

void SceneManager::Shutdown()
{
	for (Scene* scene : scenes)
	{
		scene->Shutdown();
	}
	scenes.erase(scenes.begin(), scenes.end());
}

void SceneManager::Update()
{
	for (Scene* scene : GetActiveScenes())
	{
		scene->Update();
	}
}

bool SceneManager::Render()
{
	for (Scene* scene : GetActiveScenes())
	{
		scene->Render();
	}
	return true;
}

std::vector<Scene*> SceneManager::GetActiveScenes()
{
	std::vector<Scene*> output;
	for (Scene* scene : scenes)
	{
		if (scene->GetActive())
		{
			output.push_back(scene);
		}
	}
	return output;
}

void SceneManager::AddScene(Scene* scene)
{
	scenes.push_back(scene);
}

Scene* SceneManager::GetScene(std::string name)
{
	for (Scene* scene : scenes)
	{
		if (scene->GetName() == name)
		{
			return scene;
		}
	}
	return nullptr;
}
