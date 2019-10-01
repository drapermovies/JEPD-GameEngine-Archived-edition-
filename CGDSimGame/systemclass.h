#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "inputclass.h"
#include "graphicsclass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&) = default;
	~SystemClass() = default;

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK messageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	bool Frame();
	void initializeWindows(int&, int&);
	void shutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	InputClass* m_input;
	GraphicsClass* m_graphics;
};

static LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* applicationHandle = 0;

