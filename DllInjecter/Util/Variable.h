#pragma once
#ifndef Variable_H
#define Variable_H
#include <windows.h>
namespace Var
{
	extern HWND hWnd;
	extern HINSTANCE hInstance;
	extern WNDPROC OldWndProc;
}
#endif // !Variable_H