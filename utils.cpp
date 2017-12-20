#include "stdafx.h"
#include "utils.h"
/************************************************************************* /
线程函数,显示cmd信息
/************************************************************************/
DWORD WINAPI utils::ThreadFun(LPVOID lpParam)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead, hWrite;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;        //使用系统默认的安全描述符    
	sa.bInheritHandle = TRUE;              //创建的进程继承句柄
	if (!CreatePipe(&hRead, &hWrite, &sa, 0)) //创建匿名管道
	{
		::MessageBox(NULL, "CreatePipe Failed!", "提示", MB_OK | MB_ICONWARNING);
		return 0;
	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;    //新创建进程的标准输出连在写管道一端
	si.wShowWindow = SW_HIDE;  //隐藏窗口    
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	char cmdline[1000];
	char dirline[1000];
	//sprintf(cmdline, "%s", inputCmd);
	//sprintf(dirline, "%s", inputCmd2);
	if (!CreateProcess(NULL, cmdline, NULL, NULL, TRUE, NULL, NULL, dirline, &si, &pi)) //创建子进程
	{
		::MessageBox(NULL, "CreateProcess Failed!", "提示", MB_OK | MB_ICONWARNING);
		return 0;
	}
	/*pid = pi.dwProcessId;*/
	CloseHandle(hWrite); //关闭管道句柄
	char buffer[4096] = { 0 };
	DWORD bytesRead;
	while (true)
	{
		if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == NULL) //读取管道
			break;
		CString temp;
		temp = buffer; // outpuuCmd为输出的结果
		temp += _T("\r\n");
		/*m_edit->GetWindowText(outputCmd);*/
		////显示输出信息到编辑框,并刷新窗口
		//int len = outputCmd.GetLength();
		//m_edit->SetSel(len, len);
		//outputCmd += temp;
		//m_edit->SetWindowText(outputCmd);
		//	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)m_info);
	}
	CloseHandle(hRead);
	return 1;
}
/************************************************************************/
/* 根据进程ID（PID）获取窗体句柄                                        */
/************************************************************************/
HWND utils::GetWindowHandleByPID(DWORD dwProcessID)
{
	HWND h = ::GetTopWindow(0);
	while (h)
	{
		DWORD pid = 0;
		DWORD dwTheardId = GetWindowThreadProcessId(h, &pid);
		if (dwTheardId != 0)
		{
			if (pid == dwProcessID/*your process id*/)
			{
				// here h is the handle to the window
				return h;
			}
		}
		h = ::GetNextWindow(h, GW_HWNDNEXT);
	}
	return NULL;
}
