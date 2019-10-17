#include "systemclass.h"

bool SystemClass::Initialize()
{
	int screen_width = 0;
	int screen_height = 0;

	bool result = false;

	InitializeWindows(screen_width, screen_height);

	m_input = new InputClass();
	if (!m_input) { return false; }

	m_input->Initialize();

	m_graphics = new GraphicsClass;
	if (!m_graphics) { return false; }

	result = m_graphics->Initialize(screen_width, screen_height, m_hwnd);
	if (!result) { return false; }

	return true;
}

void SystemClass::Shutdown()
{
	if (m_graphics)
	{
		m_graphics->Shutdown();
		delete m_graphics;
		m_graphics = nullptr;
	}

	if (m_input)
	{
		delete m_input;
		m_input = nullptr;
	}

	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done = false;
	bool result = false;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
	return;
}

LRESULT SystemClass::messageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_KEYDOWN:
		{
			m_input->KeyDown((unsigned int)wparam);
			return 0;
		}
		case WM_KEYUP:
		{
			m_input->KeyUp((unsigned int)wparam);
			return 0;
		}
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
	return LRESULT();
}

bool SystemClass::Frame()
{
	bool result = false;

	if (m_input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	result = m_graphics->Frame();
	if (!result)
	{
		return false;
	}
	return true;
}

void SystemClass::InitializeWindows(int& screen_width, int& screen_height)
{
	WNDCLASSEX wc;
	DEVMODE dm_screen_settings;
	int pos_x = 0;
	int pos_y = 0;

	applicationHandle = this;

	m_hInstance = GetModuleHandle(NULL);

	m_applicationName = L"NightClubSim";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screen_width = GetSystemMetrics(SM_CXSCREEN);
	screen_height = GetSystemMetrics(SM_CYSCREEN);

	if(FULL_SCREEN)
	{
		memset(&dm_screen_settings, 0, sizeof(dm_screen_settings));
		dm_screen_settings.dmSize = sizeof(dm_screen_settings);
		dm_screen_settings.dmPelsWidth = (unsigned long)screen_width;
		dm_screen_settings.dmPelsHeight = (unsigned long)screen_height;
		dm_screen_settings.dmBitsPerPel = 32;
		dm_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dm_screen_settings, CDS_FULLSCREEN);

		pos_x = 0;
		pos_y = 0;
	}
	else
	{
		screen_width = 800;
		screen_height = 600;

		pos_x = (GetSystemMetrics(SM_CXSCREEN) - screen_width) / 2;
		pos_y = (GetSystemMetrics(SM_CYSCREEN) - screen_height) / 2;
	}
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, 
			pos_x, pos_y, screen_width, screen_height, nullptr, nullptr, m_hInstance, nullptr);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL,0);
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	applicationHandle = nullptr;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_DESTROY:
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return applicationHandle->messageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}
