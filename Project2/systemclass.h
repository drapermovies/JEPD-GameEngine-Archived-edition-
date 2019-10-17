#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "inputclass.h"
#include "graphicsclass.h"

class SystemClass
{
public:
	SystemClass() = default;
	SystemClass(const SystemClass&) = default;
	~SystemClass() = default;

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK messageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	InputClass* m_input = nullptr;
	GraphicsClass* m_graphics = nullptr;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* applicationHandle = nullptr;

