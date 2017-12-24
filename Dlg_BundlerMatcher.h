#pragma once


// Dlg_BundlerMatcher 对话框

class Dlg_BundlerMatcher : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_BundlerMatcher)

public:
	Dlg_BundlerMatcher(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Dlg_BundlerMatcher();

// 对话框数据
	enum { IDD = IDD_DIALOG_BUNDLERMATCHER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
