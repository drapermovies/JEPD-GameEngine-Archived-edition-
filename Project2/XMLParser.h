#pragma once
#include "rapidxml.hpp"
#include "SceneManager.h"
#include "rapidxml_utils.hpp"

class XMLParser
{
	friend class SceneManager;
public:
	XMLParser(SceneManager&, TimerClass&, LightShaderClass&);
	~XMLParser() = default;
private:
	SceneManager& scene_manager;
	LightShaderClass& light_shader;
};