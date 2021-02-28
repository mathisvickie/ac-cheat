#include <Windows.h>
#include "classes.h"

VOID ErasePEH(VOID) { for (DWORD offset = 0x1000; offset > 0; offset -= 4) *(DWORD*)((DWORD)ErasePEH - offset) = NULL; }

DWORD WINAPI HackThread(LPVOID param) {
	
	ACgame* AC = (ACgame*)((DWORD)::GetModuleHandleA("ac_client.exe") + 0x10F4F4); //0x50F4F4
	
	while (TRUE) {
		if (::GetAsyncKeyState(0x2) && AC->count) {
			for (DWORD cn = NULL; cn < AC->count; ++cn) {
				
				if (AC->remoteplayerlist->remoteplayer[cn] == NULL || AC->remoteplayerlist->remoteplayer[cn]->health == NULL || AC->remoteplayerlist->remoteplayer[cn]->health > 100 || AC->remoteplayerlist->remoteplayer[cn]->team == AC->localplayer->team) continue;
				
				vector diff = AC->localplayer->pos;
				diff.subtract(AC->remoteplayerlist->remoteplayer[cn]->pos);
				
				angles newrot;
				newrot.aimbot(diff);

				AC->localplayer->rot = newrot;
			}
		}
		::Sleep(10);
	}
}

DWORD WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved) {

	if (dwReason == DLL_PROCESS_ATTACH) {
		//ErasePEH();
		::CreateThread(NULL, NULL, HackThread, NULL, NULL, NULL);
		::DisableThreadLibraryCalls(hModule);
	}
	return TRUE;
}
