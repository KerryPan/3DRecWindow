
// 3DRecWindow.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy3DRecWindowApp: 
// �йش����ʵ�֣������ 3DRecWindow.cpp
//

class CMy3DRecWindowApp : public CWinApp
{
public:
	CMy3DRecWindowApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy3DRecWindowApp theApp;