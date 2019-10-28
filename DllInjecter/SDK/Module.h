#pragma once
#ifndef Module_hpp
#define Module_hpp
#include <windows.h>
#include <TlHelp32.h>
/*进程名取进程ID
@parameter 进程名
*/
DWORD _stdcall ProcessName2Pid(wchar_t *ProcessName);
#endif