#include "afxwin.h"

#include"Windows.h"

class utils{

	// �����̺߳���
	static DWORD WINAPI ThreadFun(LPVOID lpParam);
	HWND GetWindowHandleByPID(DWORD dwProcessID);
};