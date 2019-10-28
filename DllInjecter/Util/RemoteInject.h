#pragma once
#ifndef RemoteInject_H
#define RemoteInject_H
#include <windows.h>
#include <TlHelp32.h>
#include "../SDK/Module.h"
/*远程线程注入
@parameter 进程名
@paramet DLL文件目录
*/
BOOL _stdcall RemoteThreadInject(wchar_t *ProcessName, wchar_t *DllPath);

#endif
