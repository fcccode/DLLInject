#include "Module.h"
/*进程名取进程ID
@parameter 进程名
*/
DWORD _stdcall ProcessName2Pid(wchar_t *ProcessName)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnap == INVALID_HANDLE_VALUE)return NULL;
	PROCESSENTRY32 Proce = { sizeof(PROCESSENTRY32) };
	Process32First(hSnap, &Proce);
	DWORD Pid = NULL;
	do
	{
		if (wcscmp(ProcessName, Proce.szExeFile) == 0) {
			Pid = Proce.th32ProcessID;
			break;
		}
	} while (Process32Next(hSnap, &Proce));
	CloseHandle(hSnap);
	return Pid;
}