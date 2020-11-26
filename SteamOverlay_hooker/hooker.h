#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <memory>
#include <Psapi.h>
#include <chrono>
#include <thread>

using namespace std;

#define INRANGE(x,a,b)	(x >= a && x <= b) 
#define GetBits(x)		(INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GetByte(x)		(GetBits(x[0]) << 4 | GetBits(x[1]))

typedef int(WINAPI* flashwindow_fn)(HWND, bool);
flashwindow_fn o_flashwindow = nullptr;

uintptr_t find_pattern_ida_style(const char* szModule, const char* szSignature)
{
	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
	uintptr_t startAddress = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);
	uintptr_t endAddress = startAddress + modInfo.SizeOfImage;
	const char* pat = szSignature;
	uintptr_t firstMatch = 0;
	for (uintptr_t pCur = startAddress; pCur < endAddress; pCur++)
	{
		if (!*pat) return firstMatch;
		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == GetByte(pat))
		{
			if (!firstMatch) firstMatch = pCur;
			if (!pat[2]) return firstMatch;
			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
			else pat += 2;
		}
		else
		{
			pat = szSignature;
			firstMatch = 0;
		}
	}

	return NULL;
}

uintptr_t rel32_fix(uintptr_t ptr) {
	auto offset = *(uintptr_t*)(ptr + 0x1);
	return (uintptr_t)(ptr + 5 + offset);
};


namespace Steam_Hooker
{
	template < typename fn = uintptr_t>
	void* hook_function(void* function, fn detour_target, void* orig_fn, int unknown_var)
	{
		auto addr_to_hook_func = find_pattern_ida_style("gameoverlayrenderer.dll", "55 8B EC 51 8B 45 10 C7");

		typedef void* (__cdecl* proxy_function)(void*, void*, void*, int);
		proxy_function do_hook_fn = (proxy_function)addr_to_hook_func;

		return do_hook_fn(function, detour_target, orig_fn, unknown_var);
	}

	// In case you are using: E8 ? ? ? ? 83 C4 08 FF 15 ? ? ? ? (unhook_function) please don't forget about rel32_fix. Thanks!
	void unhook_function(void* function, bool should_log)
	{
		auto addr_to_hook_func = find_pattern_ida_style("gameoverlayrenderer.dll", "55 8B ?? 64 ?? ?? ?? ?? ?? 6A ?? 68 ?? ?? ?? ?? 50 64 ?? ?? ?? ?? ?? ?? 81 ?? ?? ?? ?? ?? 56 8B ?? ?? 85");
	
		typedef void (__cdecl* proxy_function)(void*, bool);
		proxy_function do_unhook_fn = (proxy_function)addr_to_hook_func;
	
		return do_unhook_fn(function, should_log);
	}
}


int WINAPI winapi_flaswindow_hook(HWND window, bool state)
{
	std::cout << "Flashwindow was called!\n" << endl;
	return o_flashwindow(window, state);
}


static auto flashwindow_func = GetProcAddress(GetModuleHandleA("user32.dll"), "FlashWindow");

//hook function example
void hooks_on_dllmain()
{
	//example hooking source engine function
	// Steam_Hooker::hook_function(vfunc/detour address, hook_itself, &original_function_typedef, 0);
	Steam_Hooker::hook_function(flashwindow_func, winapi_flaswindow_hook, &o_flashwindow, 0);
}

// unload example
void unload() 
{
	Steam_Hooker::unhook_function(flashwindow_func, false);
}

