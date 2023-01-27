/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 * Copyright (c) 2021, Rayope
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "utils.h"
#include "mod.h"
#include "natives.h"
#include "rage.h"
#include "scanner.h"
#include <bitset>
#include "atArray.h"
#include "UniversalHookX/console/console.hpp"

rage::atArray<rage::scrThread*>* m_ThreadMap{};
int textDrawCount = 0;

BOOL IsMainWindow(HWND handle)
{
	return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	handle_data& data = *(handle_data*)lParam;
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	if (data.processID != process_id || !IsMainWindow(handle))
		return TRUE;
	data.windowHandle = handle;
	return FALSE;
}

// https://stackoverflow.com/questions/1888863/how-to-get-main-window-handle-from-process-id
HWND FindMainWindow(unsigned long process_id)
{
	handle_data data;
	data.processID = process_id;
	data.windowHandle = 0;
	EnumWindows(EnumWindowsCallback, (LPARAM)&data);
	return data.windowHandle;
}

void ResetTextDrawCount()
{
	textDrawCount = 0;
}

int GetTextDrawCount()
{
	return textDrawCount;
}

float TextFontHeight(float size, eFont font)
{
	return 1;
}

void DrawTextToScreen(const char* text, float x, float y, float scale, eFont font, bool alignRight, int red, int green, int blue)
{
	// WARNING: Only the first 100 calls to this function are displayed!
	UIDEBUG::_BG_SET_TEXT_SCALE(scale, scale);
	UIDEBUG::_BG_SET_TEXT_COLOR(red, green, blue, 255);
	UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", text), x, y);

	//HUD::SET_TEXT_FONT((int)font);
	/*HUD::SET_TEXT_WRAP(0.0, 1.0);
	HUD::SET_TEXT_CENTRE(false);
	HUD::SET_TEXT_RIGHT_JUSTIFY(alignRight);
	HUD::SET_TEXT_DROPSHADOW(2, 2, 0, 0, 0);
	HUD::SET_TEXT_EDGE(1, 0, 0, 0, 205);*/

	textDrawCount++;
}

void ClipInt(int& value, int min, int max)
{
	if (value < min)
		value = min;
	else if (value > max)
	{
		if (max > min)
			value = max;
		else
			value = min;
	}
}

void ClipFloat(float& value, float min, float max)
{
	if (value < min)
		value = min;
	else if (value > max)
	{
		if (max > min)
			value = max;
		else
			value = min;
	}
}

const char* BoolToStr(bool value)
{
	return value ? "True" : "False";
}

Vector3 InitVector3(float value)
{
	Vector3 vec;
	vec.x = value;
	vec.y = value;
	vec.z = value;

	return vec;
}

bool IsVersionSupportedForGlobals(eGameVersion ver)
{
	// Note: Assumed globals are the same in steam and nonsteam
	return false; 
}

void InitThreadBasket()
{
	auto sc = scanner(nullptr);
	m_ThreadMap = sc.scan("45 33 F6 85 C9 BD").Sub(4).Rip().Sub(8).As<decltype(m_ThreadMap)>();
}

uint64_t* GetThreadAddress(int localId, uint32_t scriptHash)
{
	if (localId < 0 || scriptHash == 0)
		return nullptr;

	uint64_t* localAddress = nullptr;

	for (const auto& thread : *m_ThreadMap)
	{
		if (thread->m_scriptHash == scriptHash)
		{
			if (thread != nullptr && (uint64_t*)thread->m_stack != nullptr)
			{
				localAddress = (uint64_t*)(thread->m_stack + (localId * sizeof(uintptr_t)));
				break;
			}
		}
	}

	return localAddress;
}

uint64_t* GetGlobalPtr(int globalId)
{
	if (globalId < 0)
		return nullptr;
	return getGlobalPtr(globalId);
}

char* GetCommsCharacterName(int characterI)//Position - 0xDBDC
{
	switch (characterI)
	{
	}
	return "ERROR";
}

char* GetTextMessageName(int textId) {
	switch (textId)
	{
	}
	return "UNKNOWN";
}

char* GetCallName(int callID) {
	switch (callID)
	{
	}
	return "UNKNOWN";
}
	