#include "Scene.h"

Scene::Scene(TimerClass& timer, ShaderManager& shader_manager)
	: m_timer(timer),
	m_ShaderManager(shader_manager)
	
{
}

bool Scene::Render()
{
	DirectX::XMMATRIX world_matrix;
	DirectX::XMMATRIX view_matrix;
	DirectX::XMMATRIX projection_matrix;
	DirectX::XMMATRIX ortho_matrix;

	bool result = false;

	m_directX->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->Render();

	m_directX->GetWorldMatrix(world_matrix);
	m_camera->GetViewMatrix(view_matrix);
	m_directX->GetProjectionMatrix(projection_matrix);
	m_directX->GetOrthoMatrix(ortho_matrix);

	m_Frustum->ConstructViewFrustum(projection_matrix, view_matrix);

	m_directX->TurnOnZBuffer(true);
	m_directX->ModifyCulling(true);

	for (GameObject* go : m_gameObjects)
	{
		bool RenderObject = m_Frustum->CheckRectangle(go->GetPosition().x,
													go->GetPosition().y,
													go->GetPosition().z,
													go->GetScale().x,
													go->GetScale().y,
													go->GetScale().z);
		if (RenderObject)
		{
			world_matrix = DirectX::XMMatrixRotationY(go->GetRotation().y);
			if (go != nullptr)
			{
				go->Render(m_directX->GetDeviceContext());
			}
		}
	}

	if (m_UI)
	{
		result = m_UI->Render(m_directX, &m_ShaderManager, world_matrix, view_matrix, ortho_matrix);
	}

	m_directX->EndScene();

	return result;
}

void Scene::SetName(std::string new_name)
{
	name = new_name;
	return;
}

void Scene::SetActive(bool new_active)
{
	is_active = new_active;
}
