#include "pch.h"
#include "systemclass.h"

SystemClass::SystemClass()
{
	m_input = nullptr;
	m_graphics = nullptr;
}

bool SystemClass::Initialize()
{
	int screen_width = 0;
	int screen_height = 0;

	bool result = false;

	initializeWindows(screen_width, screen_height);

	m_input = new InputClass();
	if (!m_input) { return false; }

	m_input->initialize();

	m_graphics = new GraphicsClass;
	if (!m_graphics) { return false; }

	result = m_graphics->initialize(screen_width, screen_height, m_hwnd);
	if (!result) { return false; }

	return true;
}
