#include "System.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	System* system;
	bool result;
	
	
	system = new System;
	if(!system)
		return 0;

	result = system->Initialize();
	if (!result)
		return 0;

	system->Run();
	system->Shutdown();

	delete system;
	system = nullptr;
	return 0;
}