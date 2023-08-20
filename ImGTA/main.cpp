/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 * Copyright (c) 2021, Rayope
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "script.h"

#include "main.h"

#include "imgui_impl_win32.h"

#include "natives.h"

#include <Windows.h>

DLLObject object{};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void Load();
void OnPresent(IDXGISwapChain* swap);

BOOL APIENTRY DllMain(HMODULE inst, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		scriptRegister(inst, Load);
		presentCallbackRegister((PresentCallback)OnPresent);
		break;
	case DLL_PROCESS_DETACH:
		// If unloaded dynamically
		if (lpReserved == NULL)
		{
			presentCallbackUnregister((PresentCallback)OnPresent);
			object.Unload();
			scriptUnregister(inst);
		}
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
		switch (wParam)
		{
		case VK_INSERT:
			object.ToggleOpen();
			break;
		case VK_NUMPAD1:
			CAM::DO_SCREEN_FADE_IN(100);
			break;
		case VK_NUMPAD3:
			CAM::DO_SCREEN_FADE_OUT(100);
			break;
		case VK_NUMPAD0:
			ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 0, FALSE);
			break;
		case VK_NUMPAD7:
			MISC::SET_GAME_PAUSED(FALSE);
			PLAYER::SET_PLAYER_CONTROL(PLAYER::GET_PLAYER_INDEX(), 1, 0);
			break;
		case VK_NUMPAD5:
			//object.disableAction = !object.disableAction;
			break;
		case VK_END:
			object.PassCurrentMission();
			break;
		default:
			break;
		}
			
	}

	if (object.GetIsOpen())
		ImGui_ImplWin32_WndProcHandler(hand, msg, wParam, lParam);

	return CallWindowProcW((WNDPROC)object.GetOldProc(), hand, msg, wParam, lParam);
}

void OnPresent(IDXGISwapChain* swap)
{
	object.OnPresent(swap);
}
