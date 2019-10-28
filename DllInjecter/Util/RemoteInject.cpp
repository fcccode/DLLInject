#include "RemoteInject.h"

//远程线程注入
BOOL _stdcall RemoteThreadInject(wchar_t *ProcessName, wchar_t *DllPath)
{
	DWORD Pid = ProcessName2Pid(ProcessName);
	LPVOID LoadLibrayAddr = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
	if (!LoadLibrayAddr)return FALSE;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, Pid);
	if (hProcess == INVALID_HANDLE_VALUE)return FALSE;
	LPVOID Vaddress = VirtualAllocEx(hProcess, NULL, MAX_PATH, MEM_COMMIT, PAGE_READWRITE);
	if (!Vaddress) { CloseHandle(hProcess); return FALSE; }
	size_t Len = wcslen(DllPath) * sizeof(wchar_t) + 1;
	if (!WriteProcessMemory(hProcess, Vaddress, DllPath, Len, NULL)) { CloseHandle(hProcess); return FALSE; }
	HANDLE hRemote = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibrayAddr, Vaddress, NULL, NULL);
	if (WaitForSingleObject(hRemote, 3000) != WAIT_OBJECT_0) { CloseHandle(hRemote); CloseHandle(hProcess); return FALSE; }
	VirtualFreeEx(hProcess, Vaddress, NULL, MEM_RELEASE);
	CloseHandle(hProcess);
	CloseHandle(hRemote);
	return TRUE;
}
