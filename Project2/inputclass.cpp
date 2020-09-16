#include "inputclass.h"

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screen_width, int screen_height)
{
	HRESULT result;

	m_ScreenWidth = screen_width;
	m_ScreenHeight = screen_height;

	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
								(void**)&m_DirectInput, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	result = m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	result = m_Keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	result = m_Keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = m_Keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	result = m_DirectInput->CreateDevice(GUID_SysMouse, &m_Mouse, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	result = m_Mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	result = m_Mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = m_Mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void InputClass::Shutdown()
{
	if (m_Mouse)
	{
		m_Mouse->Unacquire();
		m_Mouse->Release();
		m_Mouse = nullptr;
	}
	if (m_Keyboard)
	{
		m_Keyboard->Unacquire();
		m_Keyboard->Release();
		m_Keyboard = nullptr;
	}
	if (m_DirectInput)
	{
		m_DirectInput->Release();
		m_DirectInput = nullptr;
	}
}

bool InputClass::Frame()
{
	bool result = ReadKeyboard();
	if (result)
	{
		result = ReadMouse();
		if (result)
		{
			ProcessInput();
		}
	}
	return result;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;

	result = m_Keyboard->GetDeviceState(sizeof(m_keys), (LPVOID)&m_keys);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			//Regain keyboard control
			m_Keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;

	result = m_Mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_MouseState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			//Try to regain mouse control
			m_Mouse->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

void InputClass::ProcessInput()
{
	m_MouseX += m_MouseState.lX;
	m_MouseY += m_MouseState.lY;

	if (m_MouseY < 0) { m_MouseY = 0; }
	if (m_MouseX < 0) { m_MouseX = 0; }

	if (m_MouseY > m_ScreenHeight) { m_MouseY = m_ScreenHeight; }
	if (m_MouseX > m_ScreenWidth) { m_MouseX = m_ScreenWidth; };
}

bool InputClass::IsKeyDown(KEY_STATE KeyState)
{
	switch (KeyState)
	{
		case KEY_STATE::ESCAPE:
		{
			//Escape key is pressed
			if (m_keys[DIK_ESCAPE] & 0x80)
			{
				return true;
			}
		}
	}
	return false;
}

void InputClass::GetMouseLocation(int& mouse_x, int& mouse_y)
{
	mouse_x = m_MouseX;
	mouse_y = m_MouseY;
}