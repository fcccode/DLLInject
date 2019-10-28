#include "Main.h"
//获取进程列表
void GetProcessList()
{
	SendDlgItemMessage(Var::hWnd, IDC_COMBO_ProcessList, CB_RESETCONTENT, NULL, NULL);
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 ProcessEntry32 = { sizeof(PROCESSENTRY32) };
	Process32First(hSnapShot, &ProcessEntry32);
	do 
	{
		SendDlgItemMessage(Var::hWnd, IDC_COMBO_ProcessList, CB_ADDSTRING, NULL, (LPARAM)ProcessEntry32.szExeFile);
	} while (Process32Next(hSnapShot, &ProcessEntry32));
	SendDlgItemMessage(Var::hWnd, IDC_COMBO_ProcessList, CB_SETCURSEL, NULL, NULL);
}
//获取注入类型
void GetInjectType()
{
	wchar_t InjectType[][256] = { L"APC注入",L"远程线程注入" };
	for (int i = 0; i < 2; i++)
	{
		SendDlgItemMessage(Var::hWnd, IDC_COMBO_InjectType, CB_ADDSTRING, NULL, (LPARAM)InjectType[i]);
	}
	SendDlgItemMessage(Var::hWnd, IDC_COMBO_InjectType, CB_SETCURSEL, NULL, NULL);
}
//选择Dll文件目录
BOOL _stdcall SelectDllFile(wchar_t *FileBuf, UINT BufLen)
{
	OPENFILENAME OpenFIle = { 0 };
	OpenFIle.lStructSize = sizeof(OPENFILENAME);
	OpenFIle.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	OpenFIle.hwndOwner = Var::hWnd;
	OpenFIle.lpstrFile = FileBuf;
	OpenFIle.nMaxFile = BufLen;
	OpenFIle.nFilterIndex = 1;
	OpenFIle.lpstrFilter = L"DLL文件(*.dll)\0*.dll";
	OpenFIle.lpstrTitle = L"打开DLL文件";
	return GetOpenFileName(&OpenFIle);
}

//窗户消息处理
BOOL _stdcall DiaProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	wchar_t FileBuf[MAX_PATH] = { 0 };
	wchar_t ProcessName[MAX_PATH] = { 0 };
	UINT nIndex = NULL;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		Var::hWnd = hWnd;
		GetProcessList();
		GetInjectType();
		//允许窗口被拖放;
		DragAcceptFiles(hWnd, TRUE);
		//设置消息过滤权限.防止因UAC管理员启动而使拖放消息被屏蔽
		ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
		ChangeWindowMessageFilter(WM_COPYDATA, MSGFLT_ADD);
		ChangeWindowMessageFilter(0x0049, MSGFLT_ADD); //0x0049 == WM_COPYGLOBALDATA;
		break;
	case WM_CLOSE:
		PostQuitMessage(NULL);
		EndDialog(hWnd, NULL);
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case IDC_BUTTON_Inject:
			GetDlgItemText(hWnd, IDC_EDIT_DllPath, FileBuf, sizeof(FileBuf));
			nIndex = SendDlgItemMessage(hWnd, IDC_COMBO_ProcessList, CB_GETCURSEL, NULL, NULL);
			SendDlgItemMessage(hWnd, IDC_COMBO_ProcessList, CB_GETLBTEXT, nIndex, (LPARAM)ProcessName);
			nIndex = SendDlgItemMessage(hWnd, IDC_COMBO_InjectType, CB_GETCURSEL, NULL, NULL);
			switch (nIndex)
			{
			case 0:
				if (APCInject(ProcessName, FileBuf)) {
					SetDlgItemText(hWnd, IDC_STATIC_OutLog, L"Tip:\r\nAPC注入成功!");
				}
				else {
					SetDlgItemText(hWnd, IDC_STATIC_OutLog, L"Tip:\r\nAPC注入失败!");
				}
				break;
			case 1:
				if (RemoteThreadInject(ProcessName, FileBuf)) {
					SetDlgItemText(hWnd, IDC_STATIC_OutLog, L"Tip:\r\n远程注入成功!");
				}
				else {
					SetDlgItemText(hWnd, IDC_STATIC_OutLog, L"Tip:\r\n远程注入失败!");
				}
				break;
			default:
				break;
			}
			break;
		case IDC_BUTTON_SelectDllFIle:
			SelectDllFile(FileBuf, sizeof(FileBuf));
			SetDlgItemText(hWnd, IDC_EDIT_DllPath, FileBuf);
			break;
		default:
			break;
		}
		break;
	case WM_DROPFILES:
		DragQueryFile((HDROP)wParam, NULL, FileBuf, sizeof(FileBuf));
		DragFinish((HDROP)wParam);
		SetDlgItemText(hWnd, IDC_EDIT_DllPath, FileBuf);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
int _stdcall WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	Var::hInstance = hInstance;
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DiaProc, NULL);
	return 0;
}
