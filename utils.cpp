#include "stdafx.h"
#include "utils.h"
/************************************************************************* /
�̺߳���,��ʾcmd��Ϣ
/************************************************************************/
DWORD WINAPI utils::ThreadFun(LPVOID lpParam)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead, hWrite;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;        //ʹ��ϵͳĬ�ϵİ�ȫ������    
	sa.bInheritHandle = TRUE;              //�����Ľ��̼̳о��
	if (!CreatePipe(&hRead, &hWrite, &sa, 0)) //���������ܵ�
	{
		::MessageBox(NULL, "CreatePipe Failed!", "��ʾ", MB_OK | MB_ICONWARNING);
		return 0;
	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;    //�´������̵ı�׼�������д�ܵ�һ��
	si.wShowWindow = SW_HIDE;  //���ش���    
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	char cmdline[1000];
	char dirline[1000];
	//sprintf(cmdline, "%s", inputCmd);
	//sprintf(dirline, "%s", inputCmd2);
	if (!CreateProcess(NULL, cmdline, NULL, NULL, TRUE, NULL, NULL, dirline, &si, &pi)) //�����ӽ���
	{
		::MessageBox(NULL, "CreateProcess Failed!", "��ʾ", MB_OK | MB_ICONWARNING);
		return 0;
	}
	/*pid = pi.dwProcessId;*/
	CloseHandle(hWrite); //�رչܵ����
	char buffer[4096] = { 0 };
	DWORD bytesRead;
	while (true)
	{
		if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == NULL) //��ȡ�ܵ�
			break;
		CString temp;
		temp = buffer; // outpuuCmdΪ����Ľ��
		temp += _T("\r\n");
		/*m_edit->GetWindowText(outputCmd);*/
		////��ʾ�����Ϣ���༭��,��ˢ�´���
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
/* ���ݽ���ID��PID����ȡ������                                        */
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
