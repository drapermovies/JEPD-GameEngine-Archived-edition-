#include "GameObject.h"

void GameObject::Release()
{
	if (m_model)
	{
		delete m_model;
		m_model = nullptr;
	}
}

std::string GameObject::GetName()
{
	return name;
}

void GameObject::SetName(std::string new_name)
{
	name = new_name;
}

void GameObject::SetPosition(float x, float y, float z)
{
	position = DirectX::XMFLOAT3(x, y, z);
	m_model->SetPosition(x, y, z);
}

DirectX::XMFLOAT3 GameObject::GetPosition()
{
	return position;
}

void GameObject::SetRotation(float x, float y, float z)
{
	rotation = DirectX::XMFLOAT3(x, y, z);
	//TODO - MODEL ROTATION
}

DirectX::XMFLOAT3 GameObject::GetRotation()
{
	return rotation;
}

void GameObject::SetScale(float x, float y, float z)
{
	scale = DirectX::XMFLOAT3(x, y, z);
	m_model->SetScale(x, y, z);
	return;
}

DirectX::XMFLOAT3 GameObject::GetScale()
{
	return scale;
}

bool GameObject::GetTag(std::string tag_search)
{
	for (std::string tag : tags)
	{
		if (tag == tag_search)
		{
			return true;
		}
	}
	return false;
}

void GameObject::SetTag(std::string tag)
{
	if (GetTag(tag))
	{
		return;
	}
	tags.push_back(tag);
}

GameObject* GameObject::GetChild(std::string child_name)
{
	for (GameObject* child : children)
	{
		if (child->GetName() == child_name)
		{
			return child;
		}
	}
	return nullptr;
}

void GameObject::SetChild(GameObject* new_child)
{
	children.push_back(new_child);
}

void GameObject::RemoveChild(std::string new_child)
{
	int place = 0;
	for (GameObject* child : children)
	{
		if (child->GetName() == new_child)
		{
			children.erase(children.begin() + place);
			return;
		}
		place++;
	}
}

std::vector<GameObject*> GameObject::GetChildren()
{
	return children;
}

std::vector<GameObject*> GameObject::GetChildrenWithTag(std::string in_tag)
{
	std::vector<GameObject*> objs;
	for (GameObject* child : children)
	{
		if (child->GetTag(in_tag))
		{
			objs.push_back(child);
		}
	}
	return objs;
}
