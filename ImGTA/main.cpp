/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 * Copyright (c) 2021, Rayope
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "script.h"
#include "main.h"
#include "imgui_impl_win32.h"
#include <Windows.h>
#include <MinHook.h>
#include "UniversalHookX/console/console.hpp"
#include "UniversalHookX/utils/utils.hpp"
#include "UniversalHookX/hooks/hooks.hpp"

DLLObject object{};

void Load();

DWORD WINAPI OnProcessAttach(LPVOID lpParam);
DWORD WINAPI OnProcessDetach(LPVOID lpParam);

void Draw() {
	object.Draw();
}

void InitContext(HWND hwnd) {
	object.InitContext(hwnd);
}

void Load() {
	object.Load();
}

DWORD WINAPI OnProcessAttach(LPVOID lpParam) {
	std::this_thread::sleep_for(std::chrono::seconds(5)); //can't hook vulkan right away

	Console::Alloc();
	LOG("[+] Rendering backend: %s\n", U::RenderingBackendToStr());
	if (U::GetRenderingBackend() == NONE) {
		LOG("[!] Looks like you forgot to set a backend. Will unload after pressing enter...");
		std::cin.get();

		FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpParam), 0);
		return 0;
	}

	MH_Initialize();
	H::Init();

	return 0;
}

BOOL APIENTRY DllMain(HMODULE inst, DWORD reason, LPVOID lpReserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(inst);
		scriptRegister(inst, Load);

		//U::SetRenderingBackend(DIRECTX12);
		U::SetRenderingBackend(VULKAN);

		U::SetInitCallback(InitContext); //hacks, needs refactor
		U::SetRenderCallback(Draw);

		HANDLE hHandle = CreateThread(NULL, 0, OnProcessAttach, inst, 0, NULL);
		if (hHandle != NULL) {
			CloseHandle(hHandle);
		}
	}
	else if (reason == DLL_PROCESS_DETACH && !lpReserved) {
		scriptUnregister(inst);
		OnProcessDetach(NULL);
	}
	return TRUE;
}

DWORD WINAPI OnProcessDetach(LPVOID lpParam) {
	H::Free();
	MH_Uninitialize();
	Console::Free();

	return 0;
}