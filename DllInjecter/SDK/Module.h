#pragma once
#ifndef Module_hpp
#define Module_hpp
#include <windows.h>
#include <TlHelp32.h>
/*������ȡ����ID
@parameter ������
*/
DWORD _stdcall ProcessName2Pid(wchar_t *ProcessName);
#endif