#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE h_prevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* system = nullptr;
	bool result = false;

	system = new SystemClass();
	if (!system)
	{
		return 0;
	}

	result = system->Initialize();
	if (result)
	{
		system->Run();
	}

	system->Shutdown();
	delete system;
	system = nullptr;

	return 0;
}