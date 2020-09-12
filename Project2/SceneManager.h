#pragma once
#include <vector>

#include "d3dclass.h"
#include "Scene.h"
#include "TimerClass.h"

#include "XMLParser.h"

class SceneManager
{
	friend class Scene;
	friend class XMLParser;
public:
	SceneManager(TimerClass&, LightShaderClass&);
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
protected:
	TimerClass& m_timer;
private:

	std::vector<Scene*> scenes;

	std::unique_ptr<XMLParser> m_parser;
};