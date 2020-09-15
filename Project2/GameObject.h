#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>
#include "modelclass.h"

class GameObject
{
public:
	GameObject() = default;
	GameObject(const GameObject&) = default;
	~GameObject() = default;

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);

	void Release();

	void Render(ID3D11DeviceContext*);

	std::string GetName();
	void SetName(std::string);

	void SetPosition(float, float, float);
	DirectX::XMFLOAT3 GetPosition();

	void SetRotation(float, float, float);
	void SetRotation(DirectX::XMFLOAT3);
	DirectX::XMFLOAT3 GetRotation();

	void SetScale(float, float, float);
	DirectX::XMFLOAT3 GetScale();

	bool GetTag(std::string tag);
	void SetTag(std::string tag);

	GameObject* GetChild(std::string);
	void SetChild(GameObject*);
	void RemoveChild(std::string);
	std::vector<GameObject*> GetChildren();
	std::vector<GameObject*> GetChildrenWithTag(std::string);

	ModelClass* GetModel() { return m_model; }
private:
	std::string name = "Game Object";

	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	std::vector<std::string> tags;

	std::vector<GameObject*> children;

	ModelClass* m_model = nullptr;
};