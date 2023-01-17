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

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void Load();

DWORD WINAPI OnProcessAttach(LPVOID lpParam);
DWORD WINAPI OnProcessDetach(LPVOID lpParam);

BOOL APIENTRY DllMain(HMODULE inst, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(inst);
		scriptRegister(inst, Load);

		U::SetRenderingBackend(VULKAN);

		HANDLE hHandle = CreateThread(NULL, 0, OnProcessAttach, inst, 0, NULL);
		if (hHandle != NULL) {
			CloseHandle(hHandle);
		}
		break;
	case DLL_PROCESS_DETACH:
		scriptUnregister(inst);
		OnProcessDetach(NULL);
		break;
	}
	return TRUE;
}

void Load()
{
	object.Load();
}

LRESULT __stdcall WndProc(HWND hand, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_KEYDOWN)
	{
		if (wParam == VK_INSERT)
			object.ToggleOpen();
	}

	if (object.GetIsOpen())
		ImGui_ImplWin32_WndProcHandler(hand, msg, wParam, lParam);

	return CallWindowProcW((WNDPROC)object.GetOldProc(), hand, msg, wParam, lParam);
}


DWORD WINAPI OnProcessAttach(LPVOID lpParam) {
	MH_Initialize();

	Console::Alloc();
	LOG("[+] Rendering backend: %s\n", U::RenderingBackendToStr());
	if (U::GetRenderingBackend() == NONE) {
		LOG("[!] Looks like you forgot to set a backend. Will unload after pressing enter...");
		std::cin.get();

		FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpParam), 0);
		return 0;
	}

	H::Init();

	return 0;
}

DWORD WINAPI OnProcessDetach(LPVOID lpParam) {
	// If the process quits leave memory management to the OS.
	// H::bShuttingDown == true means we pressed end an must free them by ourself.
	if (H::bShuttingDown) {
		H::Free();
		MH_Uninitialize();
	}

	Console::Free();

	return 0;
}