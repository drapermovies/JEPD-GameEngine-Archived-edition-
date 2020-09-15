#include "UserInterface.h"

bool UserInterface::Initialize(D3DClass* directX, int screen_height, int screen_width)
{
	bool result = false;

	m_Font = new FontClass;
	if (!m_Font)
	{
		return result;
	}

	result = m_Font->Initialize(directX->GetDevice(), directX->GetDeviceContext(),
								(char*)"../x64/Debug/data/fontdata.txt",
								(char*)"../x64/Debug/data/font01.tga", 32.0f, 3);

	if(result)
	{ 
		m_Text = new TextObject;
		if (!result)
		{
			return result;
		}

		result = m_Text->Initialize(directX->GetDevice(), directX->GetDeviceContext(),
									screen_width, screen_height, 16, m_Font, 
									(char*)"Hello world", 10, 50, 0.0f, 1.0f, 0.0f);
	}

	return result;
}

void UserInterface::Shutdown()
{
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = nullptr;
	}
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = nullptr;
	}
}

bool UserInterface::Frame(ID3D11DeviceContext*, int, float, float, float, float, float, float)
{
	return true;
}

bool UserInterface::Render(D3DClass* directX,
						   ShaderManager* shader_manager,
						   DirectX::XMMATRIX world_matrix, 
						   DirectX::XMMATRIX view_matrix, 
						   DirectX::XMMATRIX ortho_matrix)
{
	directX->TurnOnZBuffer(false);
	directX->TurnOnAlphaBlending(true);

	m_Text->Render(directX->GetDeviceContext(), shader_manager,
					world_matrix, view_matrix, ortho_matrix,
					m_Font->GetTexture());

	directX->TurnOnAlphaBlending(false);
	directX->TurnOnZBuffer(true);

	return true;
}
