#include "SceneManager.h"

SceneManager::SceneManager(TimerClass& Timer, LightShaderClass& Shader)
	: m_timer(Timer)
{
	m_parser = std::make_unique<XMLParser>(this, Timer, Shader);
}

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
	bool result = false;
	for (Scene* scene : GetActiveScenes())
	{
		result = scene->Render();
	}
	return result;
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
	scene->m_directX = directX;
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

CameraClass* SceneManager::GetSceneCamera()
{
	for (Scene* scene : GetActiveScenes())
	{
		scene->GetCamera();
	}
	return nullptr;
}
