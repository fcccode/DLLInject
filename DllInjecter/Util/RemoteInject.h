#pragma once
#ifndef RemoteInject_H
#define RemoteInject_H
#include <windows.h>
#include <TlHelp32.h>
#include "../SDK/Module.h"
/*Զ���߳�ע��
@parameter ������
@paramet DLL�ļ�Ŀ¼
*/
BOOL _stdcall RemoteThreadInject(wchar_t *ProcessName, wchar_t *DllPath);

#endif
