#pragma once
#define DIRECTINPUT_VERSION 0x0800 //Using Direct Input 8

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

enum class KEY_STATE
{
	ESCAPE = DIK_ESCAPE
};

class InputClass
{
public:
	InputClass() = default;
	InputClass(const InputClass&) = default;
	~InputClass() = default;

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();

	void GetMouseLocation(int& mouse_x, int& mouse_y);

	bool Frame();

	bool IsKeyDown(KEY_STATE);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	IDirectInput8* m_DirectInput = nullptr;
	IDirectInputDevice8* m_Keyboard = nullptr;
	IDirectInputDevice8* m_Mouse = nullptr;

	unsigned char m_keys[256];
	DIMOUSESTATE m_MouseState;

	int m_ScreenWidth = -1, m_ScreenHeight = -1;
	int m_MouseX = -1, m_MouseY = -1;
};

