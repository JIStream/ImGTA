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

DLLObject object{};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void Load();

BOOL APIENTRY DllMain(HMODULE inst, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		scriptRegister(inst, Load);
		break;
	case DLL_PROCESS_DETACH:
		scriptUnregister(inst);
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
