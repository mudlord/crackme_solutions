#include "Common.h"
#include "DllLoader.h"
#include "DoPatch.h"
#include "PatchUtil.h"
#include "minhook/include/MinHook.h"
#include "libudis86/udis86.h"
#include "utf8conv.h"
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi")
using namespace std;
using namespace utf8util;
uAddr uBase;

extern "C" int crackedbymudlord()
{
	return 1988;
}




bool IsBadReadPtr(void* p)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	if (::VirtualQuery(p, &mbi, sizeof(mbi)))
	{
		DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ 
		| PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
		bool b = !(mbi.Protect & mask);
		// check the page is not a guard page
		if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) b = true;
		return b;
	}
	return true;
}

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <windows.h>

typedef BOOL(WINAPI* dlgitems)
(HWND   hDlg,
	int    nIDDlgItem,
	LPCSTR lpString);

dlgitems settext = NULL;
BOOL _stdcall  hooksetwindow(
	HWND   hDlg,
	int    nIDDlgItem,
	LPCSTR lpString)
{
	
	return settext(hDlg,nIDDlgItem, "Cracked by mudlord, take that Markus ;)");
}

typedef int(WINAPI* MessageBoxA2)
(HWND   hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT   uType
	);
MessageBoxA2 timeout = NULL;

int _stdcall  message(HWND   hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT   uType
)
{
	return 0;
}

void DLL_patch(HMODULE base) {
	uBase = uAddr(base);
	PIMAGE_DOS_HEADER pDsHeader = PIMAGE_DOS_HEADER(uBase);
	PIMAGE_NT_HEADERS pPeHeader = PIMAGE_NT_HEADERS(uAddr(uBase) + pDsHeader->e_lfanew);
	auto hExecutableInstance = (size_t)GetModuleHandle(NULL);
	IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)(hExecutableInstance + ((IMAGE_DOS_HEADER*)hExecutableInstance)->e_lfanew);
	SIZE_T size = ntHeader->OptionalHeader.SizeOfImage;
	DWORD oldProtect;
	VirtualProtect((VOID*)hExecutableInstance, size, PAGE_EXECUTE_READWRITE, &oldProtect);

	dprintf("Let's patch!\n");
	wchar_t szExePath[MAX_PATH];
	GetModuleFileName(nullptr, szExePath, MAX_PATH);
	dwprintf(L"szExePath: %s\n", szExePath);
	if (0 != wcscmp(wcsrchr(szExePath, L'\\'), L"\\crackme.exe")) {
		return;
	}
	uBase = uAddr(base);

	MH_Initialize();
	MH_CreateHookApiEx(L"user32.dll", "SetDlgItemTextA", &hooksetwindow, reinterpret_cast<LPVOID*>(&settext), NULL);
	MH_CreateHookApiEx(L"user32.dll", "MessageBoxA", &message, reinterpret_cast<LPVOID*>(&timeout), NULL);
    MH_EnableHook(MH_ALL_HOOKS);
}