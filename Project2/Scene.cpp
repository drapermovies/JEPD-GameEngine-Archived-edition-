#include "Scene.h"

void Scene::SetName(std::string new_name)
{
	name = new_name;
	return;
}

void Scene::SetActive(bool new_active)
{
	is_active = new_active;
}
