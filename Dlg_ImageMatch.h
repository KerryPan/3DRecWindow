#pragma once

#include "3DRecWindowDlg.h"  
 

// Dlg_ImageMatch �Ի���

class Dlg_ImageMatch : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_ImageMatch)

public:
	Dlg_ImageMatch(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Dlg_ImageMatch();

	Mat matchImage[100];
	int matchsize;
	int matchnum;

	void DrawPicToHDC(Mat img, UINT ID);


// �Ի�������
	enum { IDD = IDD_DIALOG_IMAGEMATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedPic66();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnPaint();
	afx_msg void OnStnDblclickPic66();
};
