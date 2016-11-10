#include <Windows.h>

#include <sstream>
#include <vector>

#include <string.h>

using namespace std;

namespace {
vector<string> split(const string &input, char delim)
{
	istringstream stream(input);

	string str;
	vector<string> values;
	while (getline(stream, str, delim)) {
		values.push_back(str);
	}

	return values;
}
}

extern "C" {
char __declspec(dllexport) *hookStart(char *hookpath_pid)
{
	const char delim = ';';

	vector<string> values = split(hookpath_pid, delim);
	string path = values[0];
	int pid = atoi(values[1].c_str());

	char dllpath[1024];
	strcpy(dllpath, path.c_str());
	
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	void *datamemory = VirtualAllocEx(hProcess, NULL, sizeof(dllpath), MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, datamemory, (void *)dllpath, sizeof(dllpath), NULL);

	HMODULE kernel32 = GetModuleHandle("kernel32");
	FARPROC loadlibrary = GetProcAddress(kernel32, "LoadLibraryA");
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadlibrary, datamemory, 0, NULL);
	if (!hThread) {
		// 32 bit (WOW64) -> 64 bit (Native) won't work
		char errmsg[512];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errmsg, sizeof(errmsg), NULL);
		char str[1024];
		sprintf(str, "hook failed: %hs", errmsg);
		return str;
	}

	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	VirtualFreeEx(hProcess, datamemory, sizeof(dllpath), MEM_RELEASE);

	return "hook ok";
}
}

