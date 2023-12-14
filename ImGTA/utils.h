/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 * Copyright (c) 2021, Rayope
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include "types.h"
#include "enums.h"
#include "main.h"
#include "injector/injector.hpp"
#include "Patterns/Patterns.hh"
#include "ModUtils/Trampoline.h"

#include <Windows.h>
#include <cstdint>
#include <string>

typedef unsigned long long QWORD;

template <typename Func, typename Addr>
void
ReadCall(Addr address, Func& func)
{
	func = (Func)injector::GetBranchDestination(address).as_int();
}

template <typename Func, typename Addr>
void
ConvertCall(Addr address, Func& func)
{
	func = Func(address);
}

struct handle_data
{
	unsigned long processID;
	HWND windowHandle;
};

template<typename T>
struct GlobalArray {
	GlobalArray(int sizeAddr) : sizeAddr{ sizeAddr }, startAddr{ sizeAddr + 1 }
	{

	}

	void LoadElement()
	{
		if (sizeAddr != 0)
		{
			size = *(int*)GetGlobalPtr(sizeAddr);
			if (size > 0 && id < size)
			{
				int offset = startAddr + id * sizeof(T) / 8;

				uint64_t* p = GetGlobalPtr(offset);
				arr = *(T*)p;
				ptr = (T*)p;
			}
		}
	}

	T arr;
	T* ptr; // pointer, editable
	int id = 0;
	int size = 0;

private:
	int sizeAddr = 0;
	int startAddr = 0;
};

struct PaddedInt {
	int val = 0;
	DWORD _padding;
};

/*******************************************************/
template <typename T = void>
T*
GetRelativeReference(const std::string& pattern, int dataOffset,
	int nextInstOffset)
{
	uint8_t* addr = hook::get_pattern<uint8_t>(pattern);
	int32_t  offset = *(int32_t*)(addr + dataOffset);
	return (T*)(addr + offset + nextInstOffset);
}

/*******************************************************/
template <typename T = void>
T*
GetRelativeReference(const std::string& pattern, int dataOffset)
{
	uint32_t offset = *hook::get_pattern<uint32_t>(pattern, dataOffset);
	return (T*)(hook::getRVA(offset));
}

/*******************************************************/
/* Returns a random element from a container           */
/*******************************************************/
template <typename T>
auto&
GetRandomElement(const T& container)
{
	auto it = std::begin(container);
	std::advance(it, RandomSize(std::size(container) - 1));

	return *it;
}

template <typename T>
auto&
GetRandomElementMut(T& container)
{
	auto it = std::begin(container);
	std::advance(it, RandomSize(std::size(container) - 1));

	return *it;
}

template <typename Map, typename Key>
auto
LookupMap(Map& m, const Key& k)
{
	auto it = m.find(k);
	return it != m.end() ? &it->second : nullptr;
}

/*******************************************************/
template <typename T, typename C, typename O>
inline T*
GetAtOffset(C* classInst, O offset)
{
	return (T*)(reinterpret_cast<char*> (classInst) + offset);
}

/*******************************************************/
template <typename T>
std::pair<void*, const T&>
GetPatternsAndData(
	std::vector<std::tuple<std::string, int, const T&>> patterns)
{
	for (auto& [pattern, offset, data] : patterns)
	{
		hook::pattern p(pattern);
		if (p.size())
			return std::pair(p.get_first(offset), data);
	}

	return std::pair(nullptr, T());
}

/*******************************************************/
inline void*
GetPatterns(std::vector<std::pair<std::string, int>> patterns)
{
	for (auto& [pattern, offset] : patterns)
	{
		hook::pattern p(pattern);
		if (p.size())
			return p.get_first(offset);
	}

	return nullptr;
}

/*******************************************************/
template <typename T, typename V>
bool
DoesElementExist(const T& container, const V val)
{
	return std::find(std::begin(container), std::end(container), val)
		!= std::end(container);
}

/*******************************************************/
template <bool Jmp = false, typename F>
void
RegisterHook(void* addr, F hookedFunc)
{
	if constexpr (Jmp)
		injector::MakeJMP(addr, Trampoline::MakeTrampoline(
			GetModuleHandle(nullptr))
			->Jump(hookedFunc));
	else
		injector::MakeCALL(addr, Trampoline::MakeTrampoline(
			GetModuleHandle(nullptr))
			->Jump(hookedFunc));
}

/*******************************************************/
template <bool Jmp = false, typename F, typename O>
void
RegisterHook(void* addr, O& originalFunc, F hookedFunc)
{
	ReadCall(addr, originalFunc);
	RegisterHook<Jmp>(addr, hookedFunc);
}

/*******************************************************/
template <uint32_t offset, typename F, typename O>
void
RegisterHookVft(void* vft, O& originalFunc, F hookedFunc)
{
	void*& address = reinterpret_cast<void**> (vft)[offset];

	injector::WriteMemory(&originalFunc, address);
	injector::WriteMemory(&address, hookedFunc);
}

/*******************************************************/
template <typename T, uint32_t offset, typename F, typename O>
void
RegisterHookVft(O& originalFunc, F hookedFunc)
{
	RegisterHookVft<offset>((void*)T::vftable, originalFunc, hookedFunc);
}

/*******************************************************/
template <bool Jmp = false, typename F, typename O>
void
RegisterHook(const std::string& pattern, int offset, O& originalFunc,
	F hookedFunc)
{
	RegisterHook<Jmp>(hook::get_pattern(pattern, offset), originalFunc,
		hookedFunc);
}

/*******************************************************/
template <bool Jmp = false, typename F>
void
RegisterHook(const std::string& pattern, int offset, F hookedFunc)
{
	void* addr = hook::get_pattern(pattern, offset);
	RegisterHook<Jmp>(addr, hookedFunc);
}

/*******************************************************/
template<typename F, typename O>
void
RegisterHookOperand(const std::string& pattern, int offset, F hookedFunc, O& originalFunc)
{
	hook::pattern p(pattern);
	auto result = p.get_one();

	injector::WriteMemory(originalFunc, result.get<char>(offset + 4)
		+ *result.get<int32_t>(offset));

	*result.get<int32_t>(offset)
		= Trampoline::MakeTrampoline(GetModuleHandle(nullptr))
		->Jump(hookedFunc) - result.get<char>(offset + 4);
}

/*******************************************************/
void RegisterJmpHook(void* addr, void* dst, void** outOrignal, int size);

template <int size, typename F, typename O>
inline void
RegisterJmpHook(void* addr, O& originalFunc, F hookedFunc)
{
	static_assert (
		size >= 12,
		"Size should be greater than the JMP instruction (12 bytes)");

	RegisterJmpHook(addr, (void*)hookedFunc, (void**)&originalFunc, size);
}

template <int size, typename F, typename O>
inline void
RegisterJmpHook(const std::string& pattern, int offset, O& originalFunc,
	F hookedFunc)
{
	RegisterJmpHook<size>(hook::get_pattern(pattern, offset), originalFunc,
		hookedFunc);
}

/*******************************************************/
void MakeJMP64(injector::memory_pointer_tr  at,
	injector::memory_pointer_raw dest);

/*******************************************************/
template <typename T = void>
void*
SearchBack(const std::string& pattern, const std::string& pattern2,
	int max_offset, int offset = 0)
{
	puts(pattern.c_str());
	injector::memory_pointer_raw addr = hook::get_pattern(pattern);

	return hook::make_range_pattern((addr - max_offset).as_int(),
		addr.as_int(), pattern2)
		.get_one()
		.get<T>(offset);
}

/*******************************************************/
/* Macro to facilitate a hook
   Define a function like this:

   template<auto &O>
   Ret HookedFunction (...)
   {
   }

   Then register the hook like
   REGISTER_HOOK (Pattern, Offset, HookedFunction, Signature);
*/
/*******************************************************/
#define REGISTER_HOOK(pattern, offset, function, ret, ...)                     \
    {                                                                          \
        static ret (*F) (__VA_ARGS__);                                         \
        RegisterHook (pattern, offset, F, function<F>);                        \
    }

#define REGISTER_HOOK_OPERAND(pattern, offset, function, ret, ...)             \
    {                                                                          \
        static ret (*F) (__VA_ARGS__);                                         \
        RegisterHookOperand (pattern, offset, F, function<F>);                 \
    }

/*******************************************************/
#define REGISTER_HOOK_JMP(pattern, offset, function, ret, ...)                 \
    {                                                                          \
        static ret (*F) (__VA_ARGS__);                                         \
        RegisterHook<true> (pattern, offset, F, function<F>);                  \
    }

/*******************************************************/
#define REGISTER_JMP_HOOK(size, pattern, offset, function, ret, ...)           \
    {                                                                          \
        static ret (*F) (__VA_ARGS__);                                         \
        RegisterJmpHook<size> (pattern, offset, F, function<F>);               \
    }

/*******************************************************/
#define REGISTER_VFT_HOOK(type, offset, function, ret, ...)                    \
    {                                                                          \
        static ret (*F) (__VA_ARGS__);                                         \
        RegisterHookVft<type, offset> (F, function<F>);                        \
    }

BOOL IsMainWindow(HWND handle);
BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam);
HWND FindMainWindow(unsigned long processID);
void ResetTextDrawCount();
int GetTextDrawCount();
float TextFontHeight(float size, eFont font);
// WARNING DrawTextToScreen: Only the first 100 calls to this function are displayed!
void DrawTextToScreen(const char* text, float x, float y, float scale, eFont font, bool alignRight = false, int red = 255, int green = 255, int blue = 255);
void ClipInt(int& value, int min, int max);
void ClipFloat(float& value, float min, float max);
const char* BoolToStr(bool value);
Vector3 InitVector3(float value);
bool IsVersionSupportedForGlobals(eGameVersion ver);

char* GetCommsCharacterName(int characterId);
char* GetTextMessageName(int textId);
char* GetCallName(int callID);
// Thanks Parik (explanation on where and how to find addresses as well as thread structures...
//				 everything you see in these two functions)
// Thanks Gogsi123 (how to get the value from addresses in C++)
void InitThreadBasket();
uint64_t* GetThreadAddress(int localId, int scriptHash);
uint64_t* GetGlobalPtr(int globalId);
std::string GetGameVersionString();