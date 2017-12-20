#include "afxwin.h"

#include"Windows.h"

class utils{

	// 声明线程函数
	static DWORD WINAPI ThreadFun(LPVOID lpParam);
	HWND GetWindowHandleByPID(DWORD dwProcessID);
};