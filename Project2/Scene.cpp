#include "Scene.h"

Scene::Scene(TimerClass& timer, LightShaderClass& lightShader)
	: m_timer(timer),
	m_LightShader(lightShader)
{
}

bool Scene::Render()
{
	DirectX::XMMATRIX world_matrix;
	DirectX::XMMATRIX view_matrix;
	DirectX::XMMATRIX projection_matrix;

	bool result = false;

	m_directX->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->Render();

	m_directX->GetWorldMatrix(world_matrix);
	m_camera->GetViewMatrix(view_matrix);
	m_directX->GetProjectionMatrix(projection_matrix);

	for (GameObject* go : m_gameObjects)
	{
		world_matrix = DirectX::XMMatrixRotationY(go->GetRotation().y);
		if (go != nullptr)
		{
			go->Render(m_directX->GetDeviceContext());

			result = m_LightShader.Render(m_directX->GetDeviceContext(),
											go->GetModel()->GetIndexCount(),
											world_matrix, view_matrix, projection_matrix,
											go->GetModel()->GetTexture(),
											m_lightSources[0]->GetDirection(),
											m_lightSources[0]->GetAmbientColour(),
											m_lightSources[0]->GetDiffuseColour());
		}
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
