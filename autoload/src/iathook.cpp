// 関数のIATを書き換えるAPIフック
// original) ももいろテクノロジー - IAT書き換えによるAPIフックをやってみる
//           http://inaz2.hatenablog.com/entry/2015/08/10/193103

#include <Windows.h>

#include <tlhelp32.h> // CreateToolhelp32Snapshot
#include <Dbghelp.h>  // ImageDirectoryEntryToData

#pragma comment(lib, "Dbghelp")

namespace {
void modifyIATonemod(char *modname, void *origaddr, void *newaddr, HMODULE hModule)
{
	ULONG ulSize;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hModule, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);
	if (pImportDesc == NULL) {
		return;
	}

	// seek the target DLL
	while (pImportDesc->Name) {
		char *name = (char*)hModule + pImportDesc->Name;
		if (lstrcmpi(name, modname) == 0) {
			break;
		}
		pImportDesc++;
	}
	if (pImportDesc->Name == 0) {
		return;
	}

	// modify corrensponding IAT entry
	PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((char *)hModule + pImportDesc->FirstThunk);
	while (pThunk->u1.Function) {
		PROC *paddr = (PROC *)&pThunk->u1.Function;
		if (*paddr == origaddr) {
			DWORD flOldProtect;
			VirtualProtect(paddr, sizeof(paddr), PAGE_EXECUTE_READWRITE, &flOldProtect);
			*paddr = (PROC)newaddr;
			VirtualProtect(paddr, sizeof(paddr), flOldProtect, &flOldProtect);
		}
		pThunk++;
	}
}
}

void modifyIAT(char *modname, void *origaddr, void *newaddr)
{
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

	MODULEENTRY32 me;
	me.dwSize = sizeof(me);

	// modify IAT in all loaded modules
	BOOL bModuleResult = Module32First(hModuleSnap, &me);
	while (bModuleResult) {
		modifyIATonemod(modname, origaddr, newaddr, me.hModule);
		bModuleResult = Module32Next(hModuleSnap, &me);
	}

	CloseHandle(hModuleSnap);
}


