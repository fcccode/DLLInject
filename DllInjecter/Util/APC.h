#pragma once
#ifndef APC_H
#define APC_H
#include <windows.h>
#include <winternl.h>
#include <TlHelp32.h>
#include "../SDK/Module.h"
typedef enum _SECTION_INHERIT {
	ViewShare = 1,
	ViewUnmap = 2
} SECTION_INHERIT;
/*APC����ע��
@parameter ������
@parameter Dll�ļ�Ŀ¼
*/
BOOL _stdcall APCInject(wchar_t *ProcessName, wchar_t *DllPath);

#endif // !APC_H
