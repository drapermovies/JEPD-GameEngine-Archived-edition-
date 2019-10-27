#pragma once
#include <d3d11.h>
#include <vector>
#include "GameObject.h"

class Scene
{
public:
	virtual void Initialize() = 0;
	virtual void Shutdown() = 0;

	virtual void Update() = 0;
	virtual bool Render() = 0;

	void SetName(std::string);
	std::string GetName() { return name; }

	void SetActive(bool);
	bool GetActive() { return is_active; }
private:
	std::string name = "Scene";

	bool is_active = true;

	std::vector<GameObject*> m_gameObjects;
};