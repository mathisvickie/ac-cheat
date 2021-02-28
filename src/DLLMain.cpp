#include "INJ/ReflectiveLoader.h"
#include <windows.h>
bool DoUnload = false;
#include "Hooks.cpp"

DWORD InitialThread(LPVOID lpParam) {

	CHooks::InitializeHooks();

	while (!DoUnload)
		::Sleep(100);

	CHooks::UndoHooks();

	return NULL;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)::InitialThread, NULL, NULL, NULL);
	}
	return TRUE;
}
