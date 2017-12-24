#pragma once


// Dlg_genOption 对话框

class Dlg_genOption : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_genOption)

public:
	Dlg_genOption(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Dlg_genOption();

// 对话框数据
	enum { IDD = IDD_DIALOG_GENOPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
