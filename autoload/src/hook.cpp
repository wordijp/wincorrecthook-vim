#include <Windows.h>
#include <string>

#include "iathook.h"

//#pragma comment(lib, "User32") // MessageBox

namespace {
typedef int (*SearchPathAFunc)(LPCTSTR, LPCTSTR, LPCTSTR, DWORD, LPTSTR, LPTSTR *);
typedef int (*SearchPathWFunc)(LPCWSTR, LPCWSTR, LPCWSTR, DWORD, LPWSTR, LPWSTR *);
SearchPathAFunc origSearchPathA;
SearchPathWFunc origSearchPathW;

DWORD WINAPI newSearchPathA(
	LPCTSTR lpPath,
	LPCTSTR lpFileName,
	LPCTSTR lpExtension,
	DWORD nBufferLength,
	LPTSTR lpBuffer,
	LPTSTR *lpFilePart
)
{

	DWORD result = origSearchPathA(lpPath, lpFileName, lpExtension, nBufferLength, lpBuffer, lpFilePart);
	if (result > 0) {
		// success
		const size_t sz = strlen(lpBuffer);
		for (int i = 0; i < sz; i++) {
			if (lpBuffer[i] == '\\') lpBuffer[i] = '/';
		}
	}

	return result;
}

DWORD WINAPI newSearchPathW(
	LPCWSTR lpPath,
	LPCWSTR lpFileName,
	LPCWSTR lpExtension,
	DWORD nBufferLength,
	LPWSTR lpBuffer,
	LPWSTR *lpFilePart
)
{
	DWORD result = origSearchPathW(lpPath, lpFileName, lpExtension, nBufferLength, lpBuffer, lpFilePart);
	if (result > 0) {
		// success
		const size_t sz = wcslen(lpBuffer);
		for (int i = 0; i < sz; i++) {
			if (lpBuffer[i] == '\\') lpBuffer[i] = '/';
		}
	}

	return result;
}
}


extern "C" {
BOOL __declspec(dllexport) DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
	HMODULE kernel32 = GetModuleHandle("Kernel32");
	origSearchPathA = (SearchPathAFunc)GetProcAddress(kernel32, "SearchPathA");
	origSearchPathW = (SearchPathWFunc)GetProcAddress(kernel32, "SearchPathW");

	switch (dwReason) {
		case DLL_PROCESS_ATTACH:
			modifyIAT("Kernel32.dll", origSearchPathA, newSearchPathA);
			modifyIAT("Kernel32.dll", origSearchPathW, newSearchPathW);
			break;
		case DLL_PROCESS_DETACH:
			modifyIAT("Kernel32.dll", newSearchPathA, origSearchPathA);
			modifyIAT("Kernel32.dll", newSearchPathW, origSearchPathW);
			break;
	}

	return TRUE;
}
}

