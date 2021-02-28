#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include <gl\GL.h>
#pragma comment(lib, "opengl32.lib")

#include "VFTManager.hpp"
#include "MenuGL.hpp"
#include "geom.h"
#include "Reversed.hpp"

#include "RageBot.cpp"

/*
TO DO:
- bunny hop
- no reload
- anti aim
- fake lag + lag prediction
*/

class CHooks {
public:
	static CVFTManager* VFTManager;
	static CVFTManager* VFTManagerGL;

	static const int iSDL_GL_SwapBuffers = 248;
	typedef void(__stdcall *SDL_GL_SwapBuffers)(void);
	static CHooks::SDL_GL_SwapBuffers oSDL_GL_SwapBuffers;
	static void __stdcall hkSDL_GL_SwapBuffers(void);

	static const int iGL_RenderModel = 6;
	typedef int(__stdcall *GL_RenderModel)(int, int, int, int, int, int);
	static CHooks::GL_RenderModel oGL_RenderModel;
	static int __stdcall hkGL_RenderModel(int, int, int, int, int, int);
	
	typedef void(__stdcall *RemoveWorldLights)(void);
	typedef void(__stdcall *CalculateWorldLights)(void);
	static CHooks::RemoveWorldLights fnRemoveWorldLights;
	static CHooks::CalculateWorldLights fnCalculateWorldLights;

	static bool InitializeHooks(void);
	static bool UndoHooks(void);

	static void PatchBytes(void* addr, char* bytes) {

		auto str2byte = [](LPVOID str) {
			return ((*(BYTE*)str < 65 ? *(BYTE*)str - 48 : *(BYTE*)str - 55) << 4) | (*(BYTE*)((DWORD)str + 1) < 65 ? *(BYTE*)((DWORD)str + 1) - 48 : *(BYTE*)((DWORD)str + 1) - 55);
		};
		size_t len = (::strlen(bytes) + 1) / 3;

		DWORD OldProtect;
		::VirtualProtect(addr, len, PAGE_EXECUTE_READWRITE, &OldProtect);

		for (int i = 0; i < len; ++i)
			*(BYTE*)((int)addr + i) = (BYTE)str2byte(bytes + i * 3);

		::VirtualProtect(addr, len, OldProtect, &OldProtect);
	}
};

CVFTManager* CHooks::VFTManager;
CVFTManager* CHooks::VFTManagerGL;

CHooks::SDL_GL_SwapBuffers CHooks::oSDL_GL_SwapBuffers;
CHooks::GL_RenderModel CHooks::oGL_RenderModel;

CHooks::RemoveWorldLights CHooks::fnRemoveWorldLights = (CHooks::RemoveWorldLights)0x454EF0;
CHooks::CalculateWorldLights CHooks::fnCalculateWorldLights = (CHooks::CalculateWorldLights)0x44F040;

MenuGL::CMenu* Menu;

MenuGL::CMenu::CTab tab_main("Main");

MenuGL::CMenu::CItem i_ragebot("Rage Bot");
MenuGL::CMenu::CItem i_aimhead("Aim At Head");
MenuGL::CMenu::CItem i_accuracy("High Accuracy");

MenuGL::CMenu::CTab tab_visual("Visuals");

MenuGL::CMenu::CItem i_wallhack("WallHack");
MenuGL::CMenu::CItem i_maphack("Map & Radar Hack");
MenuGL::CMenu::CItem i_removeparticles("Remove Particles");
MenuGL::CMenu::CItem i_fullbright("Full Bright");

MenuGL::CMenu::CTab tab_misc("Misc");

MenuGL::CMenu::CItem i_flyhack("Fly Hack");
MenuGL::CMenu::CItem i_unload("Unload Cheat");

void __stdcall CHooks::hkSDL_GL_SwapBuffers(void) {

	if (Menu->show) { //Menu
		Menu->Draw();
		Menu->ProcessInput();
	}
	if (::GetAsyncKeyState(VK_INSERT) & 1)
		Menu->show = !Menu->show;

	if (Menu->tabs[0].items[0].active) //RageBot
		CRageBot::DoRageBot(Menu->tabs[0].items[1].active);

	static bool previous02 = false; //High Accuracy
	if (previous02 != Menu->tabs[0].items[2].active) {
		previous02 = Menu->tabs[0].items[2].active;

		if (previous02) {
			CHooks::PatchBytes((void*)0x4620B9, "8F"); //Spread
			CHooks::PatchBytes((void*)0x46226D, "31 C0 31 C9 90 90 90 90 90 90 90 90 90 90"); //Recoil
			CHooks::PatchBytes((void*)0x462202, "31 C9 89 4C 24 30 89 4C 24 34 89 4C 24 38 89 4C 24 3C 89 4C 24 40 89 4C 24 44 90 90"); //Kickback
		}
		else {
			CHooks::PatchBytes((void*)0x4620B9, "8E");
			CHooks::PatchBytes((void*)0x46226D, "0F BF 87 20 01 00 00 0F BF 8F 22 01 00 00");
			CHooks::PatchBytes((void*)0x462202, "8B 4C 24 30 D8 74 24 18 8B 54 24 34 89 54 24 40 89 4C 24 3C 8B 4C 24 38 89 4C 24 44");
		}
	}

	static bool previous10 = false; //WallHack
	if (previous10 != Menu->tabs[1].items[0].active) {
		previous10 = Menu->tabs[1].items[0].active;

		if (previous10)
			CHooks::PatchBytes((void*)0x414430, "90 90 90 90 90 90");
		else
			CHooks::PatchBytes((void*)0x414430, "0F 85 2F 05 00 00");
	}

	static bool previous11 = false; //Map & Radar Hack
	if (previous11 != Menu->tabs[1].items[1].active) {
		previous11 = Menu->tabs[1].items[1].active;

		if (previous11) {
			CHooks::PatchBytes((void*)0x40968F, "90 90 90 90 90 90"); //Map
			CHooks::PatchBytes((void*)0x4096A1, "90 90 90 90 90 90"); //Map
			CHooks::PatchBytes((void*)0x409FA1, "90 90 90 90 90 90"); //Radar
			CHooks::PatchBytes((void*)0x409FB3, "90 90 90 90 90 90"); //Radar
		}
		else {
			CHooks::PatchBytes((void*)0x40968F, "0F 85 65 01 00 00");
			CHooks::PatchBytes((void*)0x4096A1, "0F 85 53 01 00 00");
			CHooks::PatchBytes((void*)0x409FA1, "0F 85 5C 01 00 00");
			CHooks::PatchBytes((void*)0x409FB3, "0F 85 4A 01 00 00");
		}
	}

	static bool previous12 = false; //Remove Particles
	if (previous12 != Menu->tabs[1].items[2].active) {
		previous12 = Menu->tabs[1].items[2].active;

		if (previous12) {
			CHooks::PatchBytes((void*)0x4174D0, "C3"); //Particles
			CHooks::PatchBytes((void*)0x40448F, "EB 08"); //Water
		}
		else {
			CHooks::PatchBytes((void*)0x4174D0, "55");
			CHooks::PatchBytes((void*)0x40448F, "56 6A");
		}
	}

	static bool previous13 = false; //Full Bright
	if (previous13 != Menu->tabs[1].items[3].active) {
		previous13 = Menu->tabs[1].items[3].active;

		if (previous13)
			fnRemoveWorldLights();
		else
			fnCalculateWorldLights();
	}

	static bool previous20 = false; //Fly Hack
	if (previous20 != Menu->tabs[2].items[0].active) {
		previous20 = Menu->tabs[2].items[0].active;

		if (previous20)
			CHooks::PatchBytes((void*)0x45ADD8, "01");
		else
			CHooks::PatchBytes((void*)0x45ADD8, "00");
	}

	if (Menu->tabs[2].items[1].active) //Unload Cheat
		DoUnload = true;

	CHooks::oSDL_GL_SwapBuffers();
}

int __stdcall CHooks::hkGL_RenderModel(int param0, int param1, int param2, int param3, int param4, int param5) {

	if (Menu->tabs[1].items[0].active)
		::glDisable(GL_DEPTH_TEST);

	int RendererResult = CHooks::oGL_RenderModel(param0, param1, param2, param3, param4, param5);

	if (Menu->tabs[1].items[0].active)
		::glEnable(GL_DEPTH_TEST);

	return RendererResult;
}

bool CHooks::InitializeHooks(void) {

	CHooks::VFTManager = new CVFTManager((void**)0x4DA000, 1184);
	CHooks::oSDL_GL_SwapBuffers = CHooks::VFTManager->HookFn<CHooks::SDL_GL_SwapBuffers>(CHooks::hkSDL_GL_SwapBuffers, CHooks::iSDL_GL_SwapBuffers);

	CHooks::VFTManagerGL = new CVFTManager((void**)0x509B2C, 8);
	CHooks::oGL_RenderModel = CHooks::VFTManagerGL->HookFn<CHooks::GL_RenderModel>(CHooks::hkGL_RenderModel, CHooks::iGL_RenderModel);

	tab_main.items.push_back(i_ragebot);
	tab_main.items.push_back(i_aimhead);
	tab_main.items.push_back(i_accuracy);

	tab_visual.items.push_back(i_wallhack);
	tab_visual.items.push_back(i_maphack);
	tab_visual.items.push_back(i_removeparticles);
	tab_visual.items.push_back(i_fullbright);

	tab_misc.items.push_back(i_flyhack);
	tab_misc.items.push_back(i_unload);

	Menu = new MenuGL::CMenu(100, 100, 305, 145);
	Menu->tabs.push_back(tab_main);
	Menu->tabs.push_back(tab_visual);
	Menu->tabs.push_back(tab_misc);
	
	return true;
}

bool CHooks::UndoHooks(void) {

	CHooks::VFTManager->UnhookFn(CHooks::iSDL_GL_SwapBuffers);
	CHooks::VFTManagerGL->UnhookFn(CHooks::iGL_RenderModel);

	delete CHooks::VFTManager;
	delete CHooks::VFTManagerGL;
	delete Menu;

	return true;
}
