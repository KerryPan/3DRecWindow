#pragma once

#include "3DRecWindowDlg.h"  
 

// Dlg_ImageMatch 对话框

class Dlg_ImageMatch : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_ImageMatch)

public:
	Dlg_ImageMatch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Dlg_ImageMatch();

	Mat matchImage[100];
	int matchsize;
	int matchnum;

	void DrawPicToHDC(Mat img, UINT ID);


// 对话框数据
	enum { IDD = IDD_DIALOG_IMAGEMATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedPic66();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnPaint();
	afx_msg void OnStnDblclickPic66();
};
